#ifndef _SPC_MEMORY_H
#define _SPC_MEMORY_H

#include <stddef.h>


//===========================================================================//
//******* MEMORY SUBSYSTEM **************************************************//
// Heavily inspired by DOOM's zone-based memory manager, but leaning towards //
// the original concept of having multiple concurrent pools.                 //
//---------------------------------------------------------------------------//
// File: memory.h                                                            //
// Date: 2020-07-29                                                          //
// Author: CalmBit <calmbit@posteo.net>                                      //
//===========================================================================//

#define DEFAULT_POOL_SIZE 32*1024*1024 // 32MiB

typedef enum memory_user {
    SPC_MU_UNOWNED = 0,
    SPC_MU_WINDOW,
    SPC_MU_FILE,
    SPC_MU_GRAPHICS,
    SPC_MU_SOUND,
    SPC_MU_LEVEL,
    SPC_MU_INTERNAL,
    SPC_MU_MATH,
    SPC_MU_KEEP,
} memory_user;

typedef struct memory_block_t memory_block_t;

typedef struct memory_block_pool_t memory_block_pool_t;

memory_block_pool_t *memory_allocate_pool(size_t size);

void memory_init(void);

void *memory_alloc(memory_user owner, size_t size);

void memory_free(void *ptr);

void memory_cleanup(void);

void memory_dump(void);

const char *memory_user_name(memory_user owner);

size_t memory_real_size(memory_block_t *block);

#endif
