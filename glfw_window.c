#include "window.h"

#include "error.h"
#include "memory.h"
#include "shader.h"
#include "mmath.h"

#define GL_SILENCE_DEPRECATION

#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const struct
{
    float x, y;
} vertices[3] =
{
    { -0.6f, -0.4f},
    {  0.6f, -0.4f},
    {   0.f,  0.6f}
};
 
void err_callback(int err, const char* desc);

void window_init() {
    if(!glfwInit()) {
        error("unable to intialize GLFW");
    }
    glfwSetErrorCallback(err_callback);
}

window_t* window_create(uint16_t width, uint16_t height) {
    window_t* win;

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    win = memory_alloc(SPC_MU_WINDOW, sizeof(window_t));
    win->width = width;
    win->height = height;
    win->window = glfwCreateWindow(width, height, "SpaceGame", NULL, NULL);
    if(!win->window) {
        error("unable to initialize window");
    }

    return win;
}

void window_loop(window_t* win) {
    GLenum glew_status;
    GLuint vao, vbo;
    GLint vpos_location, m_loc, v_loc, p_loc;
    shader_t* shader;
    mat4_t model = MAT4_IDENT
    mat4_t view = MAT4_IDENT
    mat4_t proj = MAT4_IDENT
    vec3_t pos = {0,0.5,-1};
    vec3_t target = {0,0,0};
    vec3_t up = {0,1,0};
    float angle = 0.0f;
    bool back = false;

    if(win == NULL || win->window == NULL) {
        error("attempted loop before initialization of window struct");
    }

    glfwMakeContextCurrent(win->window);
    if((glew_status = glewInit()) != GLEW_OK) {
        error("unable to initialize GLEW, code: %i", glew_status);
    }
    glfwSwapInterval(1);

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
    mat4_lookAt(&pos, &target, &up, &view);
    glUniformMatrix4fv(v_loc, 1, GL_FALSE, ((float*)&view));

    p_loc = glGetUniformLocation(shader->prog, "proj");
    mat4_perspective(90.0f, 800.0f/600.0f, 0.1f, 100.0f, &proj);
    glUniformMatrix4fv(p_loc, 1, GL_TRUE, ((float*)&proj));

    while(!glfwWindowShouldClose(win->window)) {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader->prog);

        mat4_rotY(0.01f, &model);

        angle += 0.01f;
        if(angle >= 360.0f) {
            angle = 0.0f;
        }

        glUniformMatrix4fv(m_loc, 1, GL_FALSE, ((float*)&model));

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(win->window);

        glfwPollEvents();
    }

    shader_destroy(shader);
}

void window_free(window_t* win) {
    glfwDestroyWindow(win->window);
    memory_free(win);
}

void window_destroy() {
    glfwTerminate();
}

void err_callback(int err, const char* desc) {
    error("GLFW error code %i: %s", err, desc);
}
