#ifndef _SPC_OBJ_H
#define _SPC_OBJ_H

#include "mmath.h"
#include "list.h"

typedef struct obj_face_t {
    float* vert_index;
    float* uv_index;
    float* norm_index;
    size_t count;
} obj_face_t;

typedef struct obj_t {
    list_t* verticies; // vec4
    list_t* normals; // vec3
    list_t* uvs; // vec3
    list_t* faces;  // obj_face_t
} obj_t;

obj_t* obj_create(const char* path);
void obj_destroy(obj_t* obj);

#endif
