#include "memory.h"
#include "window.h"

#include <stdio.h>

int main(int argc, char** argv) {
    memory_init();
    window_init();

    window_t* win = window_create(800, 600);
    window_loop(win);

    window_free(win);

    window_destroy();
    memory_destroy();
    return 0;
}
