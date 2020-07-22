#ifndef _SPC_STR_UTIL_H
#define _SPC_STR_UTIL_H

#include <stddef.h>

typedef struct su_string_t {
    char* buffer;
    size_t len;
} su_string_t;

typedef struct su_split_node_t {
    su_string_t* str;
    struct su_split_node_t* next;
} su_split_node_t;

typedef struct su_split_t {
    su_split_node_t* base;
    size_t count;
} su_split_t;


su_split_t* su_split_string(const char* str, char delimiter, size_t max_len);


void su_string_destroy(su_string_t* buf);
void su_split_destroy(su_split_t* split);

#endif
