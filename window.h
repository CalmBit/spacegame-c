#ifndef _SPC_WINDOW_H
#define _SPC_WINDOW_H

#include <stdint.h>

typedef struct window_t {
    void* window;
    uint16_t width;
    uint16_t height;
} window_t;


void window_init();
window_t* window_create(uint16_t height, uint16_t width);
void window_free(window_t* win);
void window_loop(window_t* win);
void window_destroy();

#endif
