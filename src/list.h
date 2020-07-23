#ifndef _SPC_LIST_H
#define _SPC_LIST_H

#include <stddef.h>

#define DEFAULT_LIST_CAPACITY 8

typedef struct list_t {
    size_t capacity;
    size_t length;
    void **buff;
} list_t;

list_t *list_create(void);

void list_destroy(list_t *list);

void list_push_back(list_t *list, void *item);

void list_resize(list_t *list, size_t capacity);

#endif
