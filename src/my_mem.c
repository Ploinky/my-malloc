#include "my_mem.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

// total memory size in bytes
#define HEAP_SIZE 1024

// heap memory
static unsigned char heap[HEAP_SIZE];

// used for block ids to improve log output
static int block_counter = 0;

// a single block of memory on the heap
typedef struct mem_block_s {
    struct mem_block_s* previous_block;
    struct mem_block_s* next_block;
    size_t location; // index into the heap array?
    size_t size; // size of the block
    bool is_used;
    int block_id;
} mem_block_t;

void print_blocks(void) {
    printf("===================================\r\n");
    mem_block_t* block = (mem_block_t*) &heap[0];
    do {
        printf("block %d, %zu bytes - ", block->block_id, block->size);
        if(block->is_used) {
            printf("used\r\n");
        } else {
            printf("free\r\n");
        }
        block = block->next_block;
    } while(block != NULL);
    printf("---------------------------------------\r\n");
    
}

// zero out part of the heap memory
// @param start_index index to start deleting at, inclusive
// @param count number of bytes to zero out
void zero_memory(size_t start_index, size_t count) {
    assert(start_index + count <= HEAP_SIZE);

    for(int i = 0; i < count; i++) {
        heap[start_index + i] = 0;
    }
}

// combine two free adjacent blocks into one
// @param first_block pointer to the first block
// @param second_block pointer to the second block
// @return a pointer to the new block
void* combine_free_blocks(mem_block_t* first_block, mem_block_t* second_block) {
    // both blocks must exist
    assert(first_block != NULL);
    assert(second_block != NULL);

    // both blocks must be free
    assert(!first_block->is_used);
    assert(!second_block->is_used);

    // the blocks must be adjacent, first_block first
    assert(first_block->next_block == second_block);
    assert(second_block->previous_block == first_block);

    // block previously behind second block is now behind first block
    first_block->next_block = second_block->next_block;

    // if block exists behind second block, it is now behind first block
    if(first_block->next_block != NULL) {
        first_block->next_block->previous_block = first_block;
    }

    // first block increases in size
    first_block->size += second_block->size + sizeof(mem_block_t);

    // zero out remaining memory of second block
    zero_memory(second_block->location, second_block->size + sizeof(mem_block_t));

    return first_block;
}

void my_allocator_init(void) {
    // clear memory
    zero_memory(0, HEAP_SIZE);

    // set up the entire heap as one empty block
    mem_block_t* first_block = (mem_block_t*) &heap[0];
    first_block->previous_block = NULL;
    first_block->next_block = NULL;
    first_block->size = HEAP_SIZE - sizeof(mem_block_t);
    first_block->location = 0;
    first_block->is_used = false;
    first_block->block_id = block_counter++;
}

void* my_malloc(size_t req_size) {
    for(
        mem_block_t* existing_block = (mem_block_t*) &heap[0];  // start with block 0
        existing_block != NULL;                                 // continue through all blocks
        existing_block = existing_block->next_block             // next iteration -> next block
    ) {
        if(existing_block->is_used || existing_block->size < req_size) {
            // block is not useable for this malloc
            continue;
        }

        mem_block_t* next_block = (mem_block_t*) &heap[existing_block->location + sizeof(mem_block_t) + req_size];
        next_block->size = existing_block->size - sizeof(mem_block_t) - req_size;
        next_block->is_used = false;
        next_block->location = existing_block->location + sizeof(mem_block_t) + req_size;
        next_block->previous_block = existing_block;
        next_block->next_block = existing_block->next_block;
        next_block->block_id = block_counter++;

        existing_block->next_block = next_block;
        existing_block->is_used = true;
        existing_block->size = req_size;

        return &heap[existing_block->location + sizeof(mem_block_t)];
    }


    return NULL;
}

void my_free(void* ptr) {
    // TODO trust user ptr or follow the linked list to see if it exists?
    for(
        mem_block_t* existing_block = (mem_block_t*) &heap[0];
        existing_block != NULL;
        existing_block = existing_block->next_block
    ) {
        void* existing_block_memory_ptr = &heap[existing_block->location + sizeof(mem_block_t)];
        if(existing_block_memory_ptr != ptr) {
            continue;
        }

        // block is no longer in use
        existing_block->is_used = false;
        // zero out old data
        zero_memory(existing_block->location + sizeof(mem_block_t), existing_block->size);

        // combine this and previous block
        if(existing_block->previous_block != NULL && existing_block->previous_block->is_used == false) {
            existing_block = combine_free_blocks(existing_block->previous_block, existing_block);
        }

        // combine this and next block
        if(existing_block->next_block != NULL && existing_block->next_block->is_used == false) {
            existing_block = combine_free_blocks(existing_block, existing_block->next_block);
        }
        
        return;
    }
}