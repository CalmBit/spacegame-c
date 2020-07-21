#ifndef _SPC_MMATH_H
#define _SPC_MMATH_H

#include "flags.h"

#include <stddef.h>

typedef struct vec2_t {
    float x, y;
} vec2_t;

vec2_t* vec2_create(float x, float y);

void vec2_add(vec2_t* src, vec2_t* dst);
void vec2_sub(vec2_t* src, vec2_t* dst);
void vec2_copy(vec2_t* src, vec2_t* dst);
void vec2_sdiv(float src, vec2_t* dst);
float vec2_magnitude(vec2_t* src);
void vec2_normalize(vec2_t *dst);

typedef struct vec3_t {
    float x, y, z;
} vec3_t;

vec3_t* vec3_create(float x, float y, float z);

void vec3_add(vec3_t* src, vec3_t* dst);
void vec3_sub(vec3_t* src, vec3_t* dst);
void vec3_copy(vec3_t* src, vec3_t* dst);
void vec3_cross(vec3_t* src, vec3_t* dst);
void vec3_sdiv(float src, vec3_t* dst);
float vec3_magnitude(vec3_t* src);
void vec3_normalize(vec3_t *dst);

typedef struct vec4_t {
    float x, y, z, w;
} vec4_t;

vec4_t* vec4_create(float x, float y, float z, float w);

void vec4_add(vec4_t* src, vec4_t* dst);
void vec4_sub(vec4_t* src, vec4_t* dst);
void vec4_copy(vec4_t* src, vec4_t* dst);
void vec4_sdiv(float src, vec4_t* dst);

typedef struct mat2_t {
    float a11, a21;
    float a12, a22;
} mat2_t;

#define MAT2(a11, a12, a21, a22) { \
    a11, a21, \
    a12, a22, \
};

#define MAT2_IDENT MAT2(1, 0, \
                        0, 1)

typedef struct mat3_t {
    float a11, a21, a31;
    float a12, a22, a32;
    float a13, a23, a33;
} mat3_t;

#define MAT3(a11, a12, a13, a21, a22, a23,a31, a32, a33) { \
    a11, a21, a31, \
    a12, a22, a32, \
    a13, a23, a33 \
};


#define MAT3_IDENT MAT3(1, 0, 0, \
                        0, 1, 0, \
                        0, 0, 1)

typedef struct mat4_t {
    float a11, a21, a31, a41;
    float a12, a22, a32, a42;
    float a13, a23, a33, a43;
    float a14, a24, a34, a44;
} mat4_t;

#define MAT4(a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34, a41, a42, a43, a44) { \
    a11, a21, a31, a41, \
    a12, a22, a32, a42, \
    a13, a23, a33, a43, \
    a14, a24, a34, a44 \
};

#define MAT4_IDENT MAT4(1, 0, 0, 0, \
                        0, 1, 0, 0, \
                        0, 0, 1, 0, \
                        0, 0, 0, 1)

mat2_t* mat2_create();
void mat2_copy(mat2_t* src, mat2_t *dst);
void mat2_add(mat2_t* src, mat2_t* dst);
void mat2_scalar(float scale, mat2_t* dst);
void mat2_mul(mat2_t* src, mat2_t* dst);

mat3_t* mat3_create();
void mat3_copy(mat3_t* src, mat3_t* dst);
void mat3_add(mat3_t* src, mat3_t* dst);
void mat3_scalar(float scale, mat3_t* dst);
void mat3_mul(mat3_t* src, mat3_t* dst);

mat4_t* mat4_create();
void mat4_copy(mat4_t* src, mat4_t *dst);
void mat4_scale(vec4_t* scale, mat4_t* dst);
void mat4_mul(mat4_t* src, mat4_t* dst);
void mat4_rotX(float theta, mat4_t* dst);
void mat4_rotY(float theta, mat4_t* dst);
void mat4_rotZ(float theta, mat4_t* dst);
void mat4_translate(vec3_t* pos, mat4_t* dst);

void mat4_lookAt(vec3_t* eye, vec3_t* target, vec3_t* up, mat4_t* dst);
void mat4_perspective(float fov, float aspect, float near, float far, mat4_t *dst);
void mat4_print(mat4_t* dst);
void math_free_container(void* cont);

#endif
