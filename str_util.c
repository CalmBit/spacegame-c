#include "str_util.h"

#include "error.h"
#include "memory.h"

#include <stdio.h>
#include <stdbool.h>

su_split_t* su_split_string(const char* str, char delimiter, size_t max_len) {
    char buffer[max_len];
    bool last_delim = false;
    size_t len = 0;
    size_t index = 0;
    su_split_node_t* cur;
    su_split_t* split;

    split = memory_alloc(SPC_MU_INTERNAL, sizeof(su_split_t));
    split->base = NULL;
    split->count = 0;

    for(len = 0;len < max_len;len++) {
        if(str[len] != delimiter && str[len] != '\0') {
            buffer[index++] = str[len];
            last_delim = false;
        } 
        else if((str[len] == delimiter && !last_delim) || str[len] == '\0') {
            buffer[index++] = '\0';
            if(split->base == NULL) {
                split->base = memory_alloc(SPC_MU_INTERNAL, sizeof(su_split_node_t));
                cur = split->base;
            } else { 
                cur->next = memory_alloc(SPC_MU_INTERNAL, sizeof(su_split_node_t));
                cur = cur->next;
            }
            cur->str = memory_alloc(SPC_MU_INTERNAL, sizeof(su_string_t));
            cur->str->buffer = memory_alloc(SPC_MU_INTERNAL, index);
            // we only want the length of the string, not of its null
            // terminator.
            cur->str->len = index-1;
            for(index--;index < max_len;index--) {
                cur->str->buffer[index] = buffer[index];
            }
            split->count++;
            index = 0;
            if(str[len] == '\0') break;
        }
    }

    if(str[len] != '\0') {
        su_split_destroy(split);
        error("buffer overflow avoided in split_string, "
              "size > %i", max_len);
    }
    
    printf("found %i groups in str '%s'\n", split->count, str);

    return split;
}


void su_string_destroy(su_string_t* str) {
    memory_free((void*)str->buffer);
    memory_free(str);
}

void su_split_destroy(su_split_t* split) {
    size_t i;
    su_split_node_t* n = split->base;
    for(i = 0;i < split->count;i++) {
        split->base = n->next;
        su_string_destroy(n->str);
        memory_free(n);
        n = split->base;
    }
    memory_free(split);
}
