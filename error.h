#ifndef _SPC_ERROR_H
#define _SPC_ERROR_H

#include "flags.h"

#if defined(_SPC_DEBUG) || defined(_SPC_TRACE)
#define DEBUG(fmt, ...) debug(fmt, __VA_ARGS__);
#else
#define DEBUG(fmt, ...) 
#endif

#ifdef _SPC_TRACE
#define TRACE(fmt, ...) trace(fmt, __VA_ARGS__);
#else
#define TRACE(fmt, ...)
#endif

void error(const char* fmt, ...);
void warning(const char* fmt, ...);
void debug(const char* fmt, ...);
void trace(const char* fmt, ...);

#endif
