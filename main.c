#include "memory.h"
#include "window.h"
#include "mmath.h"

#include <stdio.h>

int main(int argc, char** argv) {
    memory_init();
    /*window_init();

    window_t* win = window_create(800, 600);
    window_loop(win);

    window_free(win);

    window_destroy();*/
    mat4_t a = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    vec3_t pos = {1, 2, 3};
    vec4_t scale = {2, 2, 2, 1};
    mat4_translate(&pos, &a);
    mat4_scale(&scale, &a);
    printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
            a.a11, a.a12, a.a13, a.a14,
            a.a21, a.a22, a.a23, a.a24,
            a.a31, a.a32, a.a33, a.a34,
            a.a41, a.a42, a.a43, a.a44);
    memory_destroy();
    return 0;
}
