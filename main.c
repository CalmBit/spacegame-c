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
    vec3_t eye = {100, 200, 300};
    vec3_t target = {75, 95, 250};
    vec3_t up = {0, 1, 0};
    mat4_t* m = mat4_create();

    mat4_lookAt(&eye, &target, &up, m);
    math_free_container(m);
    memory_destroy();
    return 0;
}
