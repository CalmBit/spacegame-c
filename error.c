#include "error.h"

#include "memory.h"
#include "window.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, fmt, args);
    va_end(args);
    window_destroy();
    memory_destroy();
    exit(-1);
}
