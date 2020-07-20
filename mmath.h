#ifndef _SPC_MMATH_H
#define _SPC_MMATH_H

#include <stddef.h>

typedef struct vec2_t {
    float x, y;
} vec2_t;

vec2_t* vec2_create(float x, float y);

typedef struct vec3_t {
    float x, y, z;
} vec3_t;

vec3_t* vec3_create(float x, float y, float z);

typedef struct vec4_t {
    float x, y, z, w;
} vec4_t;

vec4_t* vec4_create(float x, float y, float z, float w);

typedef struct mat2_t {
    float a11, a12;
    float a21, a22;
} mat2_t;

typedef struct mat3_t {
    float a11, a12, a13;
    float a21, a22, a23;
    float a31, a32, a33;
} mat3_t;

typedef struct mat4_t {
    float a11, a12, a13, a14;
    float a21, a22, a23, a24;
    float a31, a32, a33, a34;
    float a41, a42, a43, a44;
} mat4_t;

mat2_t* mat2_identity();
void mat2_add(mat2_t* src, mat2_t* dst);
void mat2_scalar(float scale, mat2_t* dst);
void mat2_mul(mat2_t* src, mat2_t* dst);

mat3_t* mat3_identity();
void mat3_add(mat3_t* src, mat3_t* dst);
void mat3_scalar(float scale, mat3_t *dst);
void mat3_mul(mat3_t* src, mat3_t* dst);

mat4_t* mat4_identity();
void mat4_scale(vec4_t* scale, mat4_t* dst);
void mat4_mul(mat4_t* src, mat4_t* dst);
void mat4_rotX(float theta, mat4_t* dst);
void mat4_rotY(float theta, mat4_t* dst);
void mat4_rotZ(float theta, mat4_t* dst);
void mat4_translate(vec3_t* pos, mat4_t* dst);


void free_container(void* cont);

#endif
