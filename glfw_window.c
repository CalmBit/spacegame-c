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
    float x, y, z;
} vertices[8] =
{
    {  -0.5f, -0.5f, -0.5f}, //0
    {  0.5f, -0.5f, -0.5f}, //1
    {  -0.5f,  0.5f, -0.5f}, //2
    {  0.5f,  0.5f, -0.5f}, //3
    {  -0.5f, -0.5f, 0.5f}, //4
    {  0.5f, -0.5f, 0.5f}, //5
    {  -0.5f,  0.5f, 0.5f}, //6
    {  0.5f,  0.5f, 0.5f}, //7
};

static const unsigned short indexes[] = {
    0, 2, 3,
    3, 1, 0,
    7, 6, 4,
    4, 5, 7,
    2, 0, 4,
    4, 6, 2,
    3, 7, 5,
    5, 1, 3,
    1, 5, 4,
    4, 0, 1,
    6, 7, 3,
    3, 2, 6
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
    GLuint vao, vbo, ebo;
    GLint vpos_location, m_loc, v_loc, p_loc;
    shader_t* shader;
    mat4_t model = MAT4_IDENT;
    mat4_t mview = MAT4_IDENT;
    mat4_t proj = MAT4_IDENT;
    vec3_t mpos = {0,1,1};
    vec3_t mtarget = {0,0,0};
    vec3_t mup = {0,1,0};

    glEnable(GL_CULL_FACE);

    if(win == NULL || win->window == NULL) {
        error("attempted loop before initialization of window struct");
    }

    audio_reverb_t* reverb = audio_reverb_create();
    audio_reverb_set_decay(reverb, 20.0f);
    audio_reverb_set_rgain(reverb, 3.16f);
    audio_reverb_set_lgain(reverb, 10.0f);
    audio_slot_reverb(reverb);
    audio_src_t* src = audio_src_create();

    alListener3f(AL_POSITION, 0.0f, 0.0f, 1.0f);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 
                    GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, 
                    GL_STATIC_DRAW);

    shader = shader_create("test.vert", "test.frag");

    glUseProgram(shader->prog);

    vpos_location = glGetAttribLocation(shader->prog, "vPos");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 
                            sizeof(vertices[0]), 
                            NULL);

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

        glDrawElements(GL_TRIANGLES, sizeof(indexes)/sizeof(unsigned short), GL_UNSIGNED_SHORT, NULL);

        glfwSwapBuffers(win->window);
    }

    audio_reverb_destroy(reverb);
    audio_src_destroy(src);
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
