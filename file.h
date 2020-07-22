#ifndef _SPC_FILE_H
#define _SPC_FILE_H

#include <stddef.h>
#include <stdio.h>

typedef struct file_t {
    FILE* handle;
    fpos_t size;
} file_t;

file_t* file_load(const char* path, const char* mode);
void file_destroy(file_t* file);
void file_read_string(file_t* file, char* buffer, size_t count);

#endif
