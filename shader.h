#ifndef _SPC_SHADER_H
#define _SPC_SHADER_H

#include <GL/glew.h>

typedef struct shader_attribute_t {
    GLint loc;
    GLint size;
    GLenum type;
    struct shader_attribute_t* next;
} shader_attribute_t;

typedef struct shader_uniform_t {
    GLint loc;
    struct shader_uniform_t* next;
} shader_uniform_t;

typedef struct shader_t {
    GLuint prog;
    GLint stride;
    int attr_count;
    int attr_total_size;
    shader_attribute_t* attribute_locations;
    shader_uniform_t* uniform_locations;
} shader_t;

shader_t* shader_create(const char* vert_file, const char* frag_file);
void shader_destroy(shader_t* shader);

#endif
