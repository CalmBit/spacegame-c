#include "memory.h"

#include "error.h"

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

memory_block_pool_t* memory_allocate_pool(size_t size) {
    memory_block_pool_t* pool;
    memory_block_t* first;

    pool = malloc(size);
    first = ((memory_block_t*)(pool + sizeof(memory_block_pool_t)));
    pool->base.next = first;
    pool->base.prev = first;
    pool->search = first;   
    pool->base.owner = SPC_MU_KEEP;

    first->owner = SPC_MU_UNOWNED;
    first->next = &pool->base;
    first->prev = &pool->base;
    first->size = size-(sizeof(memory_block_pool_t));
    return pool;
}

memory_block_pool_t* primary_pool;

void memory_init(void) {
    primary_pool = memory_allocate_pool(DEFAULT_POOL_SIZE);
}

void memory_cleanup(void) {
    memory_block_t* cur;
    size_t leaked = 0;

    cur = &primary_pool->base;
    do {
        if(cur->owner != SPC_MU_UNOWNED && cur->owner != SPC_MU_KEEP) {
            fprintf(stderr, "!!! CAUGHT LEAK !!!! ");
            fprintf(stderr, "block at %p has owner '%s' and is %zu (real %zu) "
                            "bytes long!\n",
                    (void*)cur, memory_user_name(cur->owner), 
                    cur->size, memory_real_size(cur));
            leaked += cur->size;
        }
        DEBUG("block of type '%s', size %zu\n", memory_user_name(cur->owner), cur->size);
        cur = cur->next;
    } while(cur != &primary_pool->base);
    DEBUG("deallocating primary pool with %zu bytes leftover\n", leaked);
    free(primary_pool);
}

void memory_dump(void) {
    memory_block_t* cur;
    size_t occupied = 0;

    cur = &primary_pool->base;
    do {
        if(cur->owner != SPC_MU_UNOWNED && cur->owner != SPC_MU_KEEP) {
            occupied += cur->size;
        }
        DEBUG("block of type '%s', size %zu\n", memory_user_name(cur->owner), cur->size);
        cur = cur->next;
    } while(cur != &primary_pool->base);
    DEBUG("pool has a total of %zu bytes occupied\n", occupied);
}

void* memory_alloc(memory_user owner, size_t size) {
    memory_block_t* look = primary_pool->search;
    memory_block_t* found = NULL;
    size_t max = SIZE_MAX;
    size += sizeof(memory_block_t);
    do {
        if(look->size >= size && look->owner == SPC_MU_UNOWNED && max > look->size) {
            found = look;
            break;
        }
        look = look->next;
    } while(look != primary_pool->search);

    if(found == NULL) {
        error("unable to allocate block for size of %i, out of pooled memory\n", size);
    }

    TRACE("allocation for block of size %zu (real %zu) owned by '%s'\n",
            size, size-sizeof(memory_block_t), memory_user_name(owner));

    memory_block_t* frag = (found + size);
    frag->owner = SPC_MU_UNOWNED;
    frag->prev = found;
    frag->next = found->next;
    frag->next->prev = frag;
    frag->size = (found->size - size);
    primary_pool->search = frag;

    TRACE("fragmenting block of size %zu (real %zu)\n", 
        frag->size, memory_real_size(frag));

    found->next = frag;
    found->size = size;
    found->owner = owner;

    TRACE("returning %p of size %zu\n", (void*)((char*)(found)+ sizeof(memory_block_t)), found->size);

    return (void*)((char*)(found) + sizeof(memory_block_t));
}

void memory_free(void* ptr) {
    memory_block_t* block;
    memory_block_t* cand;
    block = (memory_block_t*)((char*)(ptr) - sizeof(memory_block_t));
    if(block->owner == SPC_MU_UNOWNED) {
        error("double free!!!!");
    }
    TRACE("freeing block of size %zu (real %zu) owned by '%s'\n", 
            block->size, memory_real_size(block),
             memory_user_name(block->owner));

    // Look backward for any unallocated blocks...
    if(block->prev->owner == SPC_MU_UNOWNED) {
        cand = block->prev;
        TRACE("merging previous block of size %zu (real %zu)...", 
                cand->size, memory_real_size(cand));
        cand->size += block->size;
        TRACE("new size %zu (real %zu)\n", 
                cand->size, memory_real_size(cand));
        cand->next = block->next;
        cand->next->prev = cand;
        block->owner = SPC_MU_SOUND;

        if(primary_pool->search == block) {
            primary_pool->search = cand;
        }
        block = cand;
    }

    // ...and forward!
    if(block->next->owner == SPC_MU_UNOWNED) {
        cand = block->next;
        TRACE("merging next block of size %zu (real %zu)...", 
                cand->size, memory_real_size(cand));
        block->size += cand->size;
        TRACE("new size %zu (real %zu)\n", 
                block->size, memory_real_size(block));
        block->next = cand->next;
        block->next->prev = block;
        if(primary_pool->search == cand) {
            primary_pool->search = block;
        }
    }

    block->owner = SPC_MU_UNOWNED;

    TRACE("final freed block of size %zu (real %zu)\n", 
            block->size, memory_real_size(block));
}

const char* memory_user_name(memory_user owner) {
    return users[owner];
}

size_t memory_real_size(memory_block_t* block) {
    return block->size - sizeof(memory_block_t);
}
