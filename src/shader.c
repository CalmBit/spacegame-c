#include "shader.h"

#include "memory.h"
#include "file.h"
#include "error.h"

#include <stdio.h>
#include <errno.h>

static int log_length;
static char log_buffer[512];

shader_t *shader_create(const char *vert_file, const char *frag_file) {
    shader_t *shader;
    size_t size;
    file_t *vertex_src;
    file_t *fragment_src;
    fpos_t loc;
    GLuint vs, fs;
    GLint last_compilation_state;
    char *vertex_buffer;
    char *fragment_buffer;

    size = sizeof(shader_t);

    vertex_src = file_load(vert_file, "r");

    vertex_buffer = (char *) memory_alloc(SPC_MU_GRAPHICS, vertex_src->size);
    loc = 0;
    while (!feof(vertex_src->handle) && loc <= vertex_src->size)
        vertex_buffer[loc++] = fgetc(vertex_src->handle);
    vertex_buffer[vertex_src->size] = '\0';
    file_destroy(vertex_src);

    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const GLchar *const *) &vertex_buffer, NULL);
    memory_free(vertex_buffer);
    glCompileShader(vs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &last_compilation_state);
    if (last_compilation_state != GL_TRUE) {
        glGetShaderInfoLog(vs, 512, &log_length, log_buffer);
        error("unable to compile vertex shader:\n%s", log_buffer);
    }

    fragment_src = file_load(frag_file, "r");

    fragment_buffer = (char *) memory_alloc(SPC_MU_GRAPHICS,
                                            fragment_src->size);
    loc = 0;
    while (!feof(fragment_src->handle) && loc <= fragment_src->size)
        fragment_buffer[loc++] = fgetc(fragment_src->handle);
    fragment_buffer[fragment_src->size] = '\0';
    file_destroy(fragment_src);

    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const GLchar *const *) &fragment_buffer, NULL);
    memory_free(fragment_buffer);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &last_compilation_state);
    if (last_compilation_state != GL_TRUE) {
        glGetShaderInfoLog(fs, 512, &log_length, log_buffer);
        error("unable to compile fragment shader:\n%s", log_buffer);
    }

    shader = memory_alloc(SPC_MU_GRAPHICS, size);

    shader->prog = glCreateProgram();
    glAttachShader(shader->prog, vs);
    glAttachShader(shader->prog, fs);
    glLinkProgram(shader->prog);

    glGetProgramiv(shader->prog, GL_LINK_STATUS, &last_compilation_state);
    if (last_compilation_state != GL_TRUE) {
        memory_free(shader);
        glGetProgramInfoLog(shader->prog, 512, &log_length, log_buffer);
        error("unable to link program:\n%s", log_buffer);
    }

    return shader;
}

void shader_destroy(shader_t *shader) {
    memory_free(shader);
}
