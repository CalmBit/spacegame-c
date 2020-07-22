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
    "KEEP",
    "WINDOW",
    "FILE",
    "GRAPHICS",
    "SOUND",
    "LEVEL",
    "INTERNAL",
    "MATH"
};

typedef enum memory_user {
    SPC_MU_UNOWNED = 0,
    SPC_MU_KEEP = 1,
    SPC_MU_WINDOW = 2,
    SPC_MU_FILE = 3,
    SPC_MU_GRAPHICS = 4,
    SPC_MU_SOUND = 5,
    SPC_MU_LEVEL = 6,
    SPC_MU_INTERNAL = 7,
    SPC_MU_MATH = 8
} memory_user;

typedef struct memory_block_t {
    size_t size;
    memory_user owner;
    struct memory_block_t* next;
    struct memory_block_t* prev;
} memory_block_t;

typedef struct memory_block_pool_t {
    memory_block_t base;
    memory_block_t *search;
} memory_block_pool_t;

memory_block_pool_t* memory_allocate_pool(size_t size);
void memory_init(void);
void* memory_alloc(memory_user owner, size_t size);
void memory_free(void* ptr);
void memory_cleanup(void);
const char* memory_user_name(memory_user owner);
size_t memory_real_size(memory_block_t* block);

#endif
