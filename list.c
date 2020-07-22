#include "list.h"

#include "memory.h"
#include "error.h"

#include <stdio.h>

static void _list_deallocate_buffer(list_t* list);

list_t* list_create(void) {
    list_t* list;

    list = memory_alloc(SPC_MU_INTERNAL, sizeof(list_t));
    list->capacity = DEFAULT_LIST_CAPACITY;
    list->length = 0;
    list->buff = memory_alloc(SPC_MU_INTERNAL, sizeof(void*)*list->capacity);
    return list;
}

void list_destroy(list_t* list) {
    _list_deallocate_buffer(list);
    memory_free(list);
}

void list_push_back(list_t* list, void* item) {
    if(list->length >= list->capacity) {
        list_resize(list, list->capacity*2);
    }
    TRACE("pushing back '%p' in slot %zu\n", item, list->length);
    list->buff[list->length++] = item;
}

void list_resize(list_t* list, size_t capacity) {
    void** newbuff;
    size_t i;

    if(list->capacity > capacity) {
        list_destroy(list);
        error("requested list capacity %i is below current capacity %i",
                capacity, list->capacity);
    }

    TRACE("resizing list from %zu to %zu\n", list->capacity, capacity);

    newbuff = memory_alloc(SPC_MU_INTERNAL, sizeof(void*)*capacity);
    for(i = 0;i < list->length;i++) {
        newbuff[i] = list->buff[i];
    }
    memory_free(list->buff);
    list->buff = newbuff;
    list->capacity = capacity;
}

static void _list_deallocate_buffer(list_t* list) {
    size_t i;
    for(i = 0;i < list->length;i++) {
        memory_free(list->buff[i]);
    }
    memory_free(list->buff);
}
