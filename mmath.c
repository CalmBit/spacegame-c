#include "mmath.h"

#include "memory.h"

#include <math.h>

// TODO: So much SIMD stuff...

#define mat_at(mat, i) (*((float*)&mat[i])) 

vec2_t* vec2_create(float x, float y) {
    vec2_t* v = memory_alloc(SPC_MU_MATH, sizeof(vec2_t));
    v->x = x;
    v->y = y;
    return v;
}

vec3_t* vec3_create(float x, float y, float z) {
    vec3_t* v = memory_alloc(SPC_MU_MATH, sizeof(vec3_t));
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

vec4_t* vec4_create(float x, float y, float z, float w) {
    vec4_t* v = memory_alloc(SPC_MU_MATH, sizeof(vec4_t));
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
    return v;
}

mat2_t* mat2_identity() {
    mat2_t* mat = memory_alloc(SPC_MU_MATH, sizeof(mat2_t));
    mat->a11 = 1; mat->a12 = 0;
    mat->a21 = 0; mat->a22 = 1;
    return mat;
}

void mat2_add(mat2_t* src, mat2_t* dst) {
    dst->a11 += src->a11; dst->a12 += src->a12;
    dst->a21 += src->a21; dst->a22 += src->a22;
}

void mat2_scalar(float scale, mat2_t* dst) {
    dst->a11 *= scale; dst->a12 *= scale;
    dst->a21 *= scale; dst->a22 *= scale;
}

void mat2_mul(mat2_t* src, mat2_t* dst) {
    mat2_t res = {
        0, 0, 
        0, 0
    };
    res.a11 = src->a11 * dst->a11 +
              src->a12 * dst->a21;
    res.a12 = src->a11 * dst->a12 +
              src->a12 * dst->a22;
    res.a21 = src->a21 * dst->a11 +
              src->a22 * dst->a21;
    res.a22 = src->a21 * dst->a12 +
              src->a22 * dst->a22;
    dst->a11 = res.a11; dst->a12 = res.a12;
    dst->a21 = res.a21; dst->a22 = res.a22;
}

mat3_t* mat3_identity() {
    mat3_t* mat = memory_alloc(SPC_MU_MATH, sizeof(mat3_t));
    mat->a11 = 1; mat->a12 = 0; mat->a13 = 0;
    mat->a21 = 0; mat->a22 = 1; mat->a23 = 0;
    mat->a31 = 0; mat->a32 = 0; mat->a33 = 1;
    return mat;
}

void mat3_add(mat3_t* src, mat3_t* dst) {
    dst->a11 += src->a11; dst->a12 += src->a12; dst->a13 += src->a13;
    dst->a21 += src->a21; dst->a22 += src->a22; dst->a23 += src->a23;
    dst->a31 += src->a31; dst->a32 += src->a32; dst->a33 += src->a33;
}

void mat3_scalar(float scale, mat3_t* dst) {
    dst->a11 *= scale; dst->a12 *= scale; dst->a13 *= scale;
    dst->a21 *= scale; dst->a22 *= scale; dst->a23 *= scale;
    dst->a31 *= scale; dst->a32 *= scale; dst->a33 *= scale;
}

void mat3_mul(mat3_t* src, mat3_t* dst) {
    mat3_t res = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
    res.a11 = src->a11 * dst->a11 +
              src->a12 * dst->a21 +
              src->a13 * dst->a31;
    res.a12 = src->a11 * dst->a12 +
              src->a12 * dst->a22 +
              src->a13 * dst->a32;
    res.a13 = src->a11 * dst->a13 +
              src->a12 * dst->a23 +
              src->a13 * dst->a33;
    res.a21 = src->a21 * dst->a11 +
              src->a22 * dst->a21 +
              src->a23 * dst->a31;
    res.a22 = src->a21 * dst->a12 +
              src->a22 * dst->a22 +
              src->a23 * dst->a32;
    res.a23 = src->a21 * dst->a13 +
              src->a22 * dst->a23 +
              src->a23 * dst->a33; 
    res.a31 = src->a31 * dst->a11 +
              src->a32 * dst->a21 +
              src->a33 * dst->a31;
    res.a32 = src->a31 * dst->a12 +
              src->a32 * dst->a22 +
              src->a33 * dst->a32;
    res.a33 = src->a31 * dst->a13 +
              src->a32 * dst->a23 +
              src->a33 * dst->a33; 
    dst->a11 = res.a11; dst->a12 = res.a12; dst->a13 = res.a13;
    dst->a21 = res.a21; dst->a22 = res.a22; dst->a23 = res.a23;
    dst->a31 = res.a31; dst->a32 = res.a32; dst->a33 = res.a33;
}

mat4_t* mat4_identity() {
    mat4_t* mat = memory_alloc(SPC_MU_MATH, sizeof(mat4_t));
    mat->a11 = 1; mat->a12 = 0; mat->a13 = 0; mat->a14 = 0;
    mat->a21 = 0; mat->a22 = 1; mat->a23 = 0; mat->a24 = 0;
    mat->a31 = 0; mat->a32 = 0; mat->a33 = 1; mat->a34 = 0;
    mat->a41 = 0; mat->a42 = 0; mat->a43 = 0; mat->a44 = 1;
    return mat;
}

void mat4_scale(vec4_t* scale, mat4_t* dst) {
    dst->a11 *= scale->x;
    dst->a22 *= scale->y;
    dst->a33 *= scale->z;
    dst->a44 *= scale->w;
}

void mat4_mul(mat4_t* src, mat4_t* dst) {
    mat4_t res = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    res.a11 = src->a11 * dst->a11 +
              src->a12 * dst->a21 +
              src->a13 * dst->a31 +
              src->a14 * dst->a41;
    res.a12 = src->a11 * dst->a12 +
              src->a12 * dst->a22 +
              src->a13 * dst->a32 +
              src->a14 * dst->a42;
    res.a13 = src->a11 * dst->a13 +
              src->a12 * dst->a23 +
              src->a13 * dst->a33 +
              src->a14 * dst->a43;
    res.a14 = src->a11 * dst->a14 +
              src->a12 * dst->a24 +
              src->a13 * dst->a34 +
              src->a14 * dst->a44;
    res.a21 = src->a21 * dst->a11 +
              src->a22 * dst->a21 +
              src->a23 * dst->a31 +
              src->a24 * dst->a41;
    res.a22 = src->a21 * dst->a12 +
              src->a22 * dst->a22 +
              src->a23 * dst->a32 +
              src->a24 * dst->a42;
    res.a23 = src->a21 * dst->a13 +
              src->a22 * dst->a23 +
              src->a23 * dst->a33 +
              src->a24 * dst->a43;
    res.a24 = src->a21 * dst->a14 +
              src->a22 * dst->a24 +
              src->a23 * dst->a34 +
              src->a24 * dst->a44;
    res.a31 = src->a31 * dst->a11 +
              src->a32 * dst->a21 +
              src->a33 * dst->a31 +
              src->a34 * dst->a41;
    res.a32 = src->a31 * dst->a12 +
              src->a32 * dst->a22 +
              src->a33 * dst->a32 +
              src->a34 * dst->a42;
    res.a33 = src->a31 * dst->a13 +
              src->a32 * dst->a23 +
              src->a33 * dst->a33 +
              src->a34 * dst->a43;
    res.a34 = src->a31 * dst->a14 +
              src->a32 * dst->a24 +
              src->a33 * dst->a34 +
              src->a34 * dst->a44;
    res.a41 = src->a41 * dst->a11 +
              src->a42 * dst->a21 +
              src->a43 * dst->a31 +
              src->a44 * dst->a41;
    res.a42 = src->a41 * dst->a12 +
              src->a42 * dst->a22 +
              src->a43 * dst->a32 +
              src->a44 * dst->a42;
    res.a43 = src->a41 * dst->a13 +
              src->a42 * dst->a23 +
              src->a43 * dst->a33 +
              src->a44 * dst->a43;
    res.a44 = src->a41 * dst->a14 +
              src->a42 * dst->a24 +
              src->a43 * dst->a34 +
              src->a44 * dst->a44;
    dst->a11 = res.a11; dst->a12 = res.a12; dst->a13 = res.a13; dst->a14 = res.a14;
    dst->a21 = res.a21; dst->a22 = res.a22; dst->a23 = res.a23; dst->a24 = res.a24;
    dst->a31 = res.a31; dst->a32 = res.a32; dst->a33 = res.a33; dst->a34 = res.a34;
    dst->a41 = res.a41; dst->a42 = res.a42; dst->a43 = res.a43; dst->a44 = res.a44;
}

void mat4_rotX(float theta, mat4_t* dst) {
    mat4_t rot_mat = {
        1, 0, 0, 0,
        0, cosf(theta), -sinf(theta), 0,
        0, sinf(theta), cosf(theta), 0,
        0, 0, 0, 1
    };
    mat4_mul(&rot_mat, dst);
}


void mat4_rotY(float theta, mat4_t* dst) {
    mat4_t rot_mat = {
        cosf(theta), 0, sinf(theta), 0,
        0, 1, 0, 0,
        -sinf(theta), 0, cosf(theta), 0,
        0, 0, 0, 1
    };
    mat4_mul(&rot_mat, dst);
}

void mat4_rotZ(float theta, mat4_t* dst) {
    mat4_t rot_mat = {
        cosf(theta), -sinf(theta), 0, 0,
        sinf(theta), cosf(theta), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    mat4_mul(&rot_mat, dst);
}

void mat4_translate(vec3_t* pos, mat4_t* dst) {
    mat4_t trans_mat = {
        1, 0, 0, pos->x,
        0, 1, 0, pos->y,
        0, 0, 1, pos->z,
        0, 0, 0, 1
    };
    mat4_mul(&trans_mat, dst);
}

void free_container(void* cont) {
    memory_free(cont);
}
