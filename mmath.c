#include "mmath.h"

#include "flags.h"
#include "memory.h"
#include "error.h"

#include <stdio.h>
#include <math.h>

// TODO: So much SIMD stuff...

#define mat_at(mat, i) (*((float*)&mat[i])) 

// vec2_t

vec2_t* vec2_create(float x, float y) {
    vec2_t* v = memory_alloc(SPC_MU_MATH, sizeof(vec2_t));
    v->x = x;
    v->y = y;
    return v;
}

void vec2_add(vec2_t* src, vec2_t* dst) {
    dst->x += src->x;
    dst->y += src->y;
}

void vec2_sub(vec2_t* src, vec2_t* dst) {
    dst->x = (src->x - dst->x);
    dst->y = (src->y - dst->y);
}


void vec2_copy(vec2_t* src, vec2_t* dst) {
    dst->x = src->x;
    dst->y = src->y;
}

void vec2_sdiv(float src, vec2_t* dst) {
    dst->x /= src;
    dst->y /= src;
}

float vec2_magnitude(vec2_t* src) {
    float res;
    res += powf(src->x, 2);
    res += powf(src->y, 2);
    res = sqrtf(res);
    return res;
}

void vec2_normalize(vec2_t *dst) {
    vec2_sdiv(vec2_magnitude(dst), dst);
}

// vec3_t

vec3_t* vec3_create(float x, float y, float z) {
    vec3_t* v = memory_alloc(SPC_MU_MATH, sizeof(vec3_t));
    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

void vec3_add(vec3_t* src, vec3_t* dst) {
    dst->x += src->x;
    dst->y += src->y;
    dst->z += src->z;
}

void vec3_sub(vec3_t* src, vec3_t* dst) {
    dst->x = (src->x - dst->x);
    dst->y = (src->y - dst->y);
    dst->z = (src->z - dst->z);
}


void vec3_copy(vec3_t* src, vec3_t* dst) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

void vec3_cross(vec3_t* src, vec3_t *dst) {
    vec3_t res = {0,0,0};
    res.x = (src->y * dst->z) - (src->z * dst->y);
    res.y = (src->z * dst->x) - (src->x * dst->z);
    res.z = (src->x * dst->y) - (src->y * dst->x);
    vec3_copy(&res, dst);
}

void vec3_sdiv(float src, vec3_t* dst) {
    dst->x /= src;
    dst->y /= src;
    dst->z /= src;
}

float vec3_magnitude(vec3_t* src) {
    float res;
    res += powf(src->x, 2);
    res += powf(src->y, 2);
    res += powf(src->z, 2);
    res = sqrtf(res);
    return res;
}

void vec3_normalize(vec3_t *dst) {
    float mag = vec3_magnitude(dst);
    TRACE("mag: %f\n", mag);
    vec3_sdiv(mag, dst);
}

// vec4_t

vec4_t* vec4_create(float x, float y, float z, float w) {
    vec4_t* v = memory_alloc(SPC_MU_MATH, sizeof(vec4_t));
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
    return v;
}

void vec4_add(vec4_t* src, vec4_t* dst) {
    dst->x += src->x;
    dst->y += src->y;
    dst->z += src->z;
    dst->w += src->w;
}

void vec4_sub(vec4_t* src, vec4_t* dst) {
    dst->x = (src->x - dst->x);
    dst->y = (src->y - dst->y);
    dst->z = (src->z - dst->z);
    dst->w = (src->w - dst->w);
}

void vec4_copy(vec4_t* src, vec4_t* dst) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    dst->w= src->w;
}

void vec4_sdiv(float src, vec4_t* dst) {
    dst->x /= src;
    dst->y /= src;
    dst->z /= src;
    dst->w /= src;
}

// mat2_t

mat2_t* mat2_create() {
    mat2_t* mat = memory_alloc(SPC_MU_MATH, sizeof(mat2_t));
    mat->a11 = 1; mat->a12 = 0;
    mat->a21 = 0; mat->a22 = 1;
    return mat;
}

void mat2_copy(mat2_t* src, mat2_t* dst) {
    dst->a11 = dst->a11; dst->a12 = dst->a12;
    dst->a21 = dst->a21; dst->a22 = dst->a22;
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
    mat2_t res = MAT2(
        src->a11 * dst->a11 + src->a12 * dst->a21, 
        src->a11 * dst->a12 + src->a12 * dst->a22,
        src->a21 * dst->a11 + src->a22 * dst->a21,
        src->a21 * dst->a12 + src->a22 * dst->a22)
    mat2_copy(src, dst);
}

// mat3_t

mat3_t* mat3_create() {
    mat3_t* mat = memory_alloc(SPC_MU_MATH, sizeof(mat3_t));
    mat->a11 = 1; mat->a12 = 0; mat->a13 = 0;
    mat->a21 = 0; mat->a22 = 1; mat->a23 = 0;
    mat->a31 = 0; mat->a32 = 0; mat->a33 = 1;
    return mat;
}

void mat3_copy(mat3_t* src, mat3_t* dst) {
    dst->a11 = dst->a11; dst->a12 = dst->a12; dst->a13 = dst->a13;
    dst->a21 = dst->a21; dst->a22 = dst->a22; dst->a23 = dst->a23;
    dst->a31 = dst->a31; dst->a32 = dst->a32; dst->a33 = dst->a33;
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
    mat3_t res = MAT3(
            src->a11 * dst->a11 + src->a12 * dst->a21 +
            src->a13 * dst->a31,

            src->a11 * dst->a12 + src->a12 * dst->a22 +
            src->a13 * dst->a32,

            src->a11 * dst->a13 + src->a12 * dst->a23 +
            src->a13 * dst->a33,

            src->a21 * dst->a11 + src->a22 * dst->a21 +
            src->a23 * dst->a31,

            src->a21 * dst->a12 +src->a22 * dst->a22 +
            src->a23 * dst->a32,

            src->a21 * dst->a13 + src->a22 * dst->a23 +
            src->a23 * dst->a33,

            src->a31 * dst->a11 + src->a32 * dst->a21 +
            src->a33 * dst->a31,

            src->a31 * dst->a12 + src->a32 * dst->a22 +
            src->a33 * dst->a32,

            src->a31 * dst->a13 + src->a32 * dst->a23 +
            src->a33 * dst->a33)
    mat3_copy(src, dst);
}

// mat4_t

mat4_t* mat4_create() {
    mat4_t* mat = memory_alloc(SPC_MU_MATH, sizeof(mat4_t));
    mat->a11 = 1; mat->a12 = 0; mat->a13 = 0; mat->a14 = 0;
    mat->a21 = 0; mat->a22 = 1; mat->a23 = 0; mat->a24 = 0;
    mat->a31 = 0; mat->a32 = 0; mat->a33 = 1; mat->a34 = 0;
    mat->a41 = 0; mat->a42 = 0; mat->a43 = 0; mat->a44 = 1;
    return mat;
}

void mat4_copy(mat4_t* src, mat4_t *dst) {
    dst->a11 = src->a11; dst->a12 = src->a12; dst->a13 = src->a13; dst->a14 = src->a14;
    dst->a21 = src->a21; dst->a22 = src->a22; dst->a23 = src->a23; dst->a24 = src->a24;
    dst->a31 = src->a31; dst->a32 = src->a32; dst->a33 = src->a33; dst->a34 = src->a34;
    dst->a41 = src->a41; dst->a42 = src->a42; dst->a43 = src->a43; dst->a44 = src->a44;
}

void mat4_scale(vec4_t* scale, mat4_t* dst) {
    dst->a11 *= scale->x;
    dst->a22 *= scale->y;
    dst->a33 *= scale->z;
    dst->a44 *= scale->w;
}

void mat4_mul(mat4_t* src, mat4_t* dst) {
    mat4_t res = MAT4(
        // a11
        src->a11 * dst->a11 + src->a12 * dst->a21 +
        src->a13 * dst->a31 + src->a14 * dst->a41,
        // a12
        src->a11 * dst->a12 + src->a12 * dst->a22 +
        src->a13 * dst->a32 + src->a14 * dst->a42,
        // a13
        src->a11 * dst->a13 +  src->a12 * dst->a23 +
        src->a13 * dst->a33 + src->a14 * dst->a43,
        // a14
        src->a11 * dst->a14 + src->a12 * dst->a24 +
        src->a13 * dst->a34 + src->a14 * dst->a44,
        // a21
        src->a21 * dst->a11 + src->a22 * dst->a21 +
        src->a23 * dst->a31 +  src->a24 * dst->a41,
        // a22
        src->a21 * dst->a12 + src->a22 * dst->a22 +
        src->a23 * dst->a32 + src->a24 * dst->a42,
        // a23
        src->a21 * dst->a13 +  src->a22 * dst->a23 +
        src->a23 * dst->a33 + src->a24 * dst->a43,
        // a24
        src->a21 * dst->a14 + src->a22 * dst->a24 +
        src->a23 * dst->a34 + src->a24 * dst->a44,
        // a31
        src->a31 * dst->a11 + src->a32 * dst->a21 +
        src->a33 * dst->a31 +  src->a34 * dst->a41,
        // a32
        src->a31 * dst->a12 + src->a32 * dst->a22 +
        src->a33 * dst->a32 + src->a34 * dst->a42,
        // a33
        src->a31 * dst->a13 + src->a32 * dst->a23 +
        src->a33 * dst->a33 + src->a34 * dst->a43,
        // a34
        src->a31 * dst->a14 + src->a32 * dst->a24 +
        src->a33 * dst->a34 + src->a34 * dst->a44,
        // a41
        src->a41 * dst->a11 + src->a42 * dst->a21 +
        src->a43 * dst->a31 + src->a44 * dst->a41,
        // a42
        src->a41 * dst->a12 + src->a42 * dst->a22 +
        src->a43 * dst->a32 + src->a44 * dst->a42,
        // a43
        src->a41 * dst->a13 + src->a42 * dst->a23 +
        src->a43 * dst->a33 + src->a44 * dst->a43,
        // a44
        src->a41 * dst->a14 + src->a42 * dst->a24 +
        src->a43 * dst->a34 + src->a44 * dst->a44)
    mat4_copy(&res, dst);
    
    TRACE("a1x | %f %f %f %f\n", dst->a11, dst->a12, dst->a13, dst->a14);
    TRACE("a2x | %f %f %f %f\n", dst->a21, dst->a22, dst->a23, dst->a24);
    TRACE("a3x | %f %f %f %f\n", dst->a31, dst->a32, dst->a33, dst->a34);
    TRACE("a4x | %f %f %f %f\n", dst->a41, dst->a42, dst->a43, dst->a44);
}

void mat4_rotX(float theta, mat4_t* dst) {
    mat4_t rot_mat = MAT4(
        1, 0, 0, 0,
        0, cosf(theta), -sinf(theta), 0,
        0, sinf(theta), cosf(theta), 0,
        0, 0, 0, 1
    )
    mat4_mul(&rot_mat, dst);
}


void mat4_rotY(float theta, mat4_t* dst) {
    mat4_t rot_mat = MAT4(
        cosf(theta), 0, sinf(theta), 0,
        0, 1, 0, 0,
        -sinf(theta), 0, cosf(theta), 0,
        0, 0, 0, 1
    )
    mat4_mul(&rot_mat, dst);
}

void mat4_rotZ(float theta, mat4_t* dst) {
    mat4_t rot_mat = MAT4(
        cosf(theta), -sinf(theta), 0, 0,
        sinf(theta), cosf(theta), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    )
    mat4_mul(&rot_mat, dst);
}

void mat4_translate(vec3_t* pos, mat4_t* dst) {
    mat4_t trans_mat = MAT4(
        1, 0, 0, pos->x,
        0, 1, 0, pos->y,
        0, 0, 1, pos->z,
        0, 0, 0, 1
    )
    mat4_mul(&trans_mat, dst);
}

void mat4_lookAt(vec3_t* eye, vec3_t* target, vec3_t* up, mat4_t* dst) {
    vec3_t forward = {0,0,0};
    vec3_t left = {0,0,0};
    vec3_t n_up = {0,0,0};

    // Forward Vector
    // First, we want to set up the right side of our subtraction
    vec3_copy(eye, &forward);
    // Then subtract in place
    vec3_sub(target, &forward);
    // and normalize the result...
    vec3_normalize(&forward);

    // Left Vector
    // First, we want to set up the right side of our cross
    vec3_copy(up, &left);
    // Then cross in place
    vec3_cross(&forward, &left);
    // and normlaize the result...
    vec3_normalize(&left);

    // Normal Up Vector
    // First, we want to set up the right side of our cross
    vec3_copy(&forward, &n_up);
    // Then cross in place - result is already normalized.
    vec3_cross(&left, &n_up);


    dst->a11 = left.x;
    dst->a21 = n_up.x;
    dst->a31 = -forward.x;
    dst->a12 = left.y;
    dst->a22 = n_up.y;
    dst->a32 = -forward.y;
    dst->a13 = left.z;
    dst->a23 = n_up.z;
    dst->a33 = -forward.z;
    dst->a14 = -((left.x * eye->x) + (left.y * eye->y) + (left.z * eye->z));
    dst->a24 = -((n_up.x * eye->x) + (n_up.y * eye->y) + (n_up.z * eye->z));
    dst->a34 = ((forward.x * eye->x) + (forward.y * eye->y) + (forward.z * eye->z));
    dst->a41 = dst->a42 = dst->a43 = 0.0f;
    dst->a44 = 1.0f;

    TRACE("v1c | %f %f %f %f\n", dst->a11, dst->a21, dst->a31, dst->a41);
    TRACE("v2c | %f %f %f %f\n", dst->a12, dst->a22, dst->a32, dst->a42);
    TRACE("v3c | %f %f %f %f\n", dst->a13, dst->a23, dst->a33, dst->a43);
    TRACE("v4c | %f %f %f %f\n", dst->a14, dst->a24, dst->a34, dst->a44);
}

void mat4_perspective(float fov, float aspect, float near, float far, mat4_t* dst) {
    float f = 1.0f / tanf(fov * 0.5f);
    float fn = 1.0f / (near - far);

    dst->a11 = f/aspect;
    dst->a21 = 0.0f;
    dst->a31 = 0.0f;
    dst->a41 = 0.0f;
    dst->a12 = 0.0f;
    dst->a22 = f;
    dst->a32 = 0.0f;
    dst->a42 = 0.0f;
    dst->a13 = 0.0f;
    dst->a23 = 0.0f;
    dst->a33 = (near+far)*fn;
    dst->a43 = 2.0f * near * far * fn;
    dst->a14 = 0.0f;
    dst->a24 = 0.0f;
    dst->a34 = -1.0f;
    dst->a44 = 0;

    //mat4_print(dst);
}

void mat4_print(mat4_t* dst) {
    DEBUG("a1c | %f %f %f %f\n", dst->a11, dst->a21, dst->a31, dst->a41);
    DEBUG("a2c | %f %f %f %f\n", dst->a12, dst->a22, dst->a32, dst->a42);
    DEBUG("a3c | %f %f %f %f\n", dst->a13, dst->a23, dst->a33, dst->a43);
    DEBUG("a4c | %f %f %f %f\n", dst->a14, dst->a24, dst->a34, dst->a44);
}

// general

void math_free_container(void* cont) {
    memory_free(cont);
}
