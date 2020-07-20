#ifndef _SPC_MEMORY_H
#define _SPC_MEMORY_H

#include <stddef.h>

// Name: memory.h
// Author: CalmBit <calmbit@posteo.net>
// Description: Heavily inspired by DOOM's zone-based memory manager, but leaning towards
// the original concept of having multiple concurrent pools.
// Date: 2020-07-19

#define DEFAULT_POOL_SIZE 8*1024*1024 // 8MiB

static const char* users[] = {
    "UNOWNED",
    "WINDOW",
    "FILE",
    "GRAPHICS",
    "SOUND",
    "LEVEL",
    "MATH"
};

typedef enum memory_user {
    SPC_MU_UNOWNED = 0,
    SPC_MU_WINDOW,
    SPC_MU_FILE,
    SPC_MU_GRAPHICS,
    SPC_MU_SOUND,
    SPC_MU_LEVEL,
    SPC_MU_MATH
} memory_user;

typedef struct block_t {
    size_t size;
    memory_user owner;
    struct block_t* next;
    struct block_t* prev;
} block_t;

typedef struct pool_t {
    block_t *base;
} pool_t;

pool_t* memory_allocate_pool(size_t size);
void memory_init();
void* memory_alloc(memory_user owner, size_t size);
void memory_free(void* ptr);
void memory_destroy();
const char* memory_user_name(memory_user owner);
size_t memory_real_size(block_t* block);

#endif
