#include "memory.h"
#include "window.h"
#include "mmath.h"
#include "audio.h"
#include "str_util.h"

#include <stdio.h>

int main(int argc, char** argv) {
    su_split_t* buf;
    su_split_t** sub;
    size_t i;
    size_t j;
    su_split_node_t* n;
    su_split_node_t* sn;
    memory_init();
    /*window_init();

    window_t* win = window_create(800, 600);
    

    audio_init();

    window_loop(win);

    audio_cleanup();
    window_destroy(win);
    window_cleanup();*/
    buf = su_split_string("6/4/1 3/5/3 7/6/5", ' ', 256);
    sub = memory_alloc(SPC_MU_INTERNAL, buf->count * sizeof(buf));

    n = buf->base;
    for(i = 0;i < buf->count;i++) {
        sub[i] = su_split_string(n->str->buffer, '/', 32);
        sn = sub[i]->base;
        for(j = 0;j < sub[i]->count;j++) {
            printf("%s\n", sn->str->buffer);
            sn = sn->next;
        }
        su_split_destroy(sub[i]);
        n = n->next;
    }
    memory_free(sub);

    su_split_destroy(buf);
    memory_cleanup();
    return 0;
}
