#ifndef _SPC_OBJ_H
#define _SPC_OBJ_H

#include "mmath.h"

typedef struct obj_face_t {
    float* vert_index;
    float* uv_index;
    float* norm_index;
    size_t count;
} obj_face_t;

typedef struct obj_t {
    vec4_t* verticies;
    vec3_t* normals;
    vec2_t* uvs;
    obj_face_t* faces;
} obj_t;

void obj_create(const char* path);

#endif