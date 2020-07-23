#include "memory.h"
#include "window.h"
#include "audio.h"
#include "str_util.h"
#include "list.h"
#include "obj.h"
#include "wav.h"

#include "../cglm/cglm.h"

#include <stdio.h>
#include <stddef.h>

int main(int argc, char **argv) {
    window_t *win;
    memory_init();
    window_init();

    win = window_create(800, 600);

    audio_init();

    /*obj_t* obj = obj_create("cube.obj");
    vec4* v;
    vec3* uvn;

    printf("verts:\n");
    for(size_t i =0;i < obj->verticies->length;i++) {
        v = (vec4*)obj->verticies->buff[i];
        printf("%f %f %f %f\n", *v[0], *v[1], *v[2], *v[3]);
    }

    printf("normals:\n");
    for(size_t i =0;i < obj->normals->length;i++) {
        uvn = (vec3*)obj->normals->buff[i];
        printf("%f %f %f\n", *uvn[0],*uvn[1],*uvn[2]);
    }

    printf("uvs:\n");
    for(size_t i =0;i < obj->uvs->length;i++) {
        uvn = (vec3*)obj->uvs->buff[i];
        printf("%f %f %f\n", *uvn[0],*uvn[1],*uvn[2]);
    }

    obj_destroy(obj);*/

    window_loop(win);

    audio_cleanup();
    window_destroy(win);
    window_cleanup();
    memory_cleanup();
    return 0;
}
