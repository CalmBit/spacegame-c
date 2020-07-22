#include "memory.h"
#include "window.h"
#include "mmath.h"
#include "audio.h"
#include "str_util.h"
#include "list.h"
#include "obj.h"
#include "wav.h"

#include <stdio.h>
#include <stddef.h>

int main(int argc, char** argv) {
    window_t* win;
    memory_init();
    window_init();

    win = window_create(800, 600);

    audio_init();
    
    /*obj_t* obj = obj_create("cube.obj");
    vec4_t* v;
    vec3_t* uvn;

    printf("verts:\n");
    for(size_t i =0;i < obj->verticies->length;i++) {
        v = (vec4_t*)obj->verticies->buff[i];
        printf("%f %f %f %f\n", v->x, v->y, v->z, v->w);
    }

    printf("normals:\n");
    for(size_t i =0;i < obj->normals->length;i++) {
        uvn = (vec3_t*)obj->normals->buff[i];
        printf("%f %f %f\n", uvn->x, uvn->y, uvn->z);
    }

    printf("uvs:\n");
    for(size_t i =0;i < obj->uvs->length;i++) {
        uvn = (vec3_t*)obj->uvs->buff[i];
        printf("%f %f %f\n", uvn->x, uvn->y, uvn->z);
    }

    obj_destroy(obj);*/

    window_loop(win);

    audio_cleanup();
    window_destroy(win);
    window_cleanup();
    memory_cleanup();
    return 0;
}
