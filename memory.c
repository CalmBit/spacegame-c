#include "memory.h"

#include "error.h"

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

pool_t* memory_allocate_pool(size_t size) {
    pool_t* pool;

    pool = malloc(size);
    pool->base = ((block_t*)(pool + sizeof(pool_t)));
    pool->base->size = (size - sizeof(pool_t) - sizeof(block_t));
    pool->base->owner = SPC_MU_UNOWNED;
    pool->base->next = pool->base;
    pool->base->prev = pool->base;
    return pool;
}

pool_t* primary_pool;

void memory_init() {
    primary_pool = memory_allocate_pool(DEFAULT_POOL_SIZE);
}

void memory_destroy() {
    block_t* cur;

    cur = primary_pool->base;
    do {
        if(cur->owner != SPC_MU_UNOWNED) {
            fprintf(stderr, "!!! CAUGHT LEAK !!!! ");
            fprintf(stderr, "block at %p has owner '%s' and is %zu (real %zu) "
                            "bytes long!\n",
                    (void*)cur, memory_user_name(cur->owner), 
                    cur->size + sizeof(block_t), cur->size);
        }
        cur = cur->next;
    } while(cur != primary_pool->base);
    free(primary_pool);
}

void* memory_alloc(memory_user owner, size_t size) {
    block_t* look = primary_pool->base;
    block_t* found = NULL;
    size_t max = SIZE_MAX;
    size += sizeof(block_t);
    do {
        if(look->size >= size && look->owner == SPC_MU_UNOWNED && max > look->size) {
            found = look;
            size_t max = found->size;
        }
        look = look->next;
    } while(look != primary_pool->base);

    if(found == NULL) {
        error("unable to allocate block for size of %i, out of pooled memory\n", size);
    }

    printf("pre-allocation block size: %zu (real %zu)\n", 
            found->size, memory_real_size(found));

    printf("allocation for block of size %zu (real %zu) owned by '%s'\n",
     size - sizeof(block_t), size, memory_user_name(owner));

    block_t* frag = (found + size);
    frag->prev = found;
    frag->next = found->next;
    frag->next->prev = frag;
    frag->size = (found->size - size);

    printf("fragmenting block of size %zu (real %zu)\n", 
        frag->size, memory_real_size(frag));

    found->next = frag;
    found->size = size-sizeof(block_t);
    found->owner = owner;

    printf("returning block of size %zu (real %zu)\n", 
        found->size, memory_real_size(found));

    return (void*)((char*)(found)+ sizeof(block_t));
}

void memory_free(void* ptr) {
    block_t* block;
    bool merge;

    block = (block_t*)((char*)(ptr) - sizeof(block_t));
    printf("freeing block of size %zu (real %zu) owned by '%s'\n", 
            block->size, memory_real_size(block),
             memory_user_name(block->owner));
    while(block->prev->owner == SPC_MU_UNOWNED && block->prev != block) {
        printf("merging previous block of size %zu (real %zu)...", 
                block->prev->size, memory_real_size(block->prev));
        block->prev->size += (block->size + (sizeof(block_t)));
        printf("new size %zu (real %zu)\n", 
                block->prev->size, memory_real_size(block->prev));
        block->prev->next = block->next;
        block->next->prev = block->prev;
        if(block == primary_pool->base)
            primary_pool->base = block->prev;
        block = block->prev;
    }
    while(block->next->owner == SPC_MU_UNOWNED && block->next != block) {
        printf("merging next block of size %zu (real %zu)...", 
                block->next->size, memory_real_size(block->next));
        block->next->size += (block->size + (sizeof(block_t)));
        printf("new size %zu (real %zu)\n", 
                block->next->size, memory_real_size(block->next));
        block->next->prev = block->prev;
        block->prev->next = block->next;
        if(block == primary_pool->base)
            primary_pool->base = block->next;
        block = block->next;
    }
    block->owner = SPC_MU_UNOWNED;
    printf("final unallocated block of size %zu (real %zu)\n", 
            block->size, memory_real_size(block));
}

const char* memory_user_name(memory_user owner) {
    return users[owner];
}

size_t memory_real_size(block_t* block) {
    return block->size + sizeof(block_t);
}
