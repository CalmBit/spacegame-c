#include "window.h"

#include "error.h"
#include "memory.h"
#include "shader.h"
#include "mmath.h"
#include "audio.h"

#define GL_SILENCE_DEPRECATION

#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const struct
{
    float x, y;
} vertices[4] =
{
    {  -0.5f, -0.5f},
    {  0.5f, -0.5f},
    {  -0.5f,  0.5f},
    {  0.5f,  0.5f}
};
 
void err_callback(int err, const char* desc);

void window_init(void) {
    if(!glfwInit()) {
        error("unable to intialize GLFW");
    }
    glfwSetErrorCallback(err_callback);
}

window_t* window_create(uint16_t width, uint16_t height) {
    window_t* win;
    GLenum glew_status;

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    win = memory_alloc(SPC_MU_WINDOW, sizeof(window_t));
    win->width = width;
    win->height = height;
    win->window = glfwCreateWindow(width, height, "SpaceGame", NULL, NULL);
    if(!win->window) {
        error("unable to initialize window");
    }

    glfwMakeContextCurrent(win->window);
    glfwSwapInterval(1);
    glfwShowWindow(win->window);
    if((glew_status = glewInit()) != GLEW_OK) {
        error("unable to initialize GLEW, code: %i", glew_status);
    }

    return win;
}

void window_loop(window_t* win) {
    GLuint vao, vbo;
    GLint vpos_location, m_loc, v_loc, p_loc;
    shader_t* shader;
    mat4_t model = MAT4_IDENT;
    mat4_t mview = MAT4_IDENT;
    mat4_t proj = MAT4_IDENT;
    vec3_t mpos = {0,0,-2};
    vec3_t mtarget = {0,0,0};
    vec3_t mup = {0,1,0};

    if(win == NULL || win->window == NULL) {
        error("attempted loop before initialization of window struct");
    }

    audio_src_t* src = audio_src_create();
    audio_src_destroy(src);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    shader = shader_create("test.vert", "test.frag");

    glUseProgram(shader->prog);

    vpos_location = glGetAttribLocation(shader->prog, "vPos");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)NULL);

    m_loc = glGetUniformLocation(shader->prog, "model");

    v_loc = glGetUniformLocation(shader->prog, "view");
    mat4_lookAt(&mpos, &mtarget, &mup, &mview);
    glUniformMatrix4fv(v_loc, 1, GL_FALSE, ((float*)&mview));

    p_loc = glGetUniformLocation(shader->prog, "proj");
    mat4_perspective(90.0f, 1.3333f, 0.1f, 100.0f, &proj);
    glUniformMatrix4fv(p_loc, 1, GL_TRUE, ((float*)&proj));

    while(!glfwWindowShouldClose(win->window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader->prog);

        mat4_rotY(0.01f, &model);
        glUniformMatrix4fv(m_loc, 1, GL_FALSE, ((float*)&model));

        /*angle += 0.01f;
        if(angle >= 360.0f) {
            angle = 0.0f;
        }*/

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(win->window);
    }

    shader_destroy(shader);
}

void window_destroy(window_t* win) {
    glfwDestroyWindow(win->window);
    memory_free(win);
}

void window_cleanup(void) {
    glfwTerminate();
}

void err_callback(int err, const char* desc) {
    error("GLFW error code %i: %s", err, desc);
}
