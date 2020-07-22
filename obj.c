#include "obj.h"

#include "memory.h"
#include "file.h"
#include "list.h"
#include "str_util.h"
#include "error.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

obj_t* obj_create(const char* path) {
    obj_t* obj;
    file_t* file;
    vec4_t* v_stor;
    vec3_t* uvn_stor;
    char* mark;
    char buffer[128];

    obj = memory_alloc(SPC_MU_GRAPHICS, sizeof(obj_t));
    obj->verticies = list_create();
    obj->normals = list_create();
    obj->uvs = list_create();
    obj->faces = list_create();
    v_stor = vec4_create(0,0,0,1);
    uvn_stor = vec3_create(0,0,0);

    file = file_load(path, "r");
    
    while(!feof(file->handle)) {
        mark = fgets(buffer, 128, file->handle);
        if(mark == NULL && ferror(file->handle)) {
            error("problem parsing obj file - %s", strerror(errno));
        } else if(mark == NULL) {
            break;
        }

        while(mark[0] == ' ' || mark[0] == '\t') {
            mark++;
        }

        switch(mark[0]) {
            case '#':
                break;
            case 'v':
                mark += 2;
                switch(mark[-1]) {
                    case ' ':
                        // just a regular vertex
                        sscanf(mark, "%f %f %f %f", &v_stor->x, &v_stor->y, &v_stor->z, &v_stor->w);
                        list_push_back(obj->verticies, v_stor);
                        v_stor = vec4_create(0,0,0,1);
                        break;
                    case 't':
                        // texture uv
                        sscanf(mark, "%f %f %f", &uvn_stor->x, &uvn_stor->y, &uvn_stor->z);
                        list_push_back(obj->uvs, uvn_stor);
                        uvn_stor = vec3_create(0,0,0);
                        break;
                    case 'n':
                        // normal
                        sscanf(mark, "%f %f %f", &uvn_stor->x, &uvn_stor->y, &uvn_stor->z);
                        list_push_back(obj->normals, uvn_stor);
                        uvn_stor = vec3_create(0,0,0);
                        break;
                }
                break;
            case 'f':
                // parse faces
                break;
            default:
                break;
        }
    }

    math_free_container(v_stor);
    math_free_container(uvn_stor);
    file_destroy(file);

    return obj;
}

void obj_destroy(obj_t* obj) {
    list_destroy(obj->verticies);
    list_destroy(obj->normals);
    list_destroy(obj->uvs);
    list_destroy(obj->faces);
    memory_free(obj);
}
