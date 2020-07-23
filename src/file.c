#include "file.h"

#include "memory.h"
#include "error.h"

#include <string.h>
#include <errno.h>

file_t *file_load(const char *path, const char *mode) {
    FILE *file;
    fpos_t file_size;

    file = fopen(path, mode);
    if (file == NULL) {
        error("unable to open file '%s' - syserr '%s'", path, strerror(errno));
    }
    fseek(file, 0, SEEK_END);
    if (fgetpos(file, &file_size)) {
        error("unable to figure out size of file '%s' - syserr '%s'", path,
              strerror(errno));
    }
    fseek(file, 0, SEEK_SET);

    file_t *r = (file_t *) memory_alloc(SPC_MU_FILE, sizeof(file_t));

    r->handle = file;
    r->size = file_size;

    return r;
}

void file_destroy(file_t *file) {
    memory_free(file);
}


void file_read_string(file_t *file, char *buffer, size_t count) {
    if (file == NULL) {
        error("attempted to perform string read on NULL file!");
    }

    if (buffer == NULL) {
        error("can't file_read_string into NULL buffer!");
    }

    fread(buffer, 1, count, file->handle);
    buffer[count] = '\0';
}
