#include "memory.h"
#include "window.h"
#include "mmath.h"
#include "audio.h"

#include <stdio.h>

int main(int argc, char** argv) {
    memory_init();
    window_init();

    window_t* win = window_create(800, 600);
    

    audio_init();

    window_loop(win);
    
    audio_cleanup();
    window_destroy(win);
    window_cleanup();
    memory_cleanup();
    return 0;
}
