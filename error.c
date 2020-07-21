#include "error.h"

#include "memory.h"
#include "window.h"
#include "audio.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, fmt, args);
    va_end(args);
    audio_cleanup();
    window_cleanup();
    memory_cleanup();
    exit(-1);
}

void debug(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void trace(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
