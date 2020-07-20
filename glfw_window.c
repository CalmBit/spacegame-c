#include "window.h"

#include "error.h"
#include "memory.h"
#include "shader.h"

#define GL_SILENCE_DEPRECATION

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
    GLint vpos_location;
    shader_t* shader;

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

    while(!glfwWindowShouldClose(win->window)) {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader->prog);
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
