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
    #if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
    fprintf(stderr, "%c[31;1m[ERROR] %c[0m", 0x1b, 0x1b);
    #else
    fprintf(stderr, "[ERROR] ");
    #endif
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    memory_cleanup();
    exit(-1);
}

void warning(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    #if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
    fprintf(stderr, "%c[33;1m[WARNING] %c[0m", 0x1b, 0x1b);
    #else
    fprintf(stderr, "[WARNING] ");
    #endif
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
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
