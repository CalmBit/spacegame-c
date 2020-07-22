#include "obj.h"

#include "file.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

void obj_create(const char* path) {
    file_t* file;
    char* mark;
    char buffer[1024];

    file = file_load(path, "r");
    
    while(!feof(file->handle)) {
        mark = fgets(buffer, 1024, file);
        if(mark == NULL && ferror(file->handle)) {
            error("problem parsing obj file - %s", strerror(errno));
        }

        switch(buffer[0]) {
            case '#':
                break;
            case 'v':
                if(buffer[1] == ' ') {
                    // just a regular vertex
                    mark = &buffer[2];
                }
                break;
            case 'f':
                // parse faces
                break;
            
        }
    }
}