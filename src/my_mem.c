#include "my_mem.h"

#include <stdio.h>
#include <stdbool.h>

static int block_counter = 0;

// a single block of memory on the heap
typedef struct mem_block_s {
    struct mem_block_s* last_block;
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

void my_allocator_init(void) {
    mem_block_t* first_block = (mem_block_t*) &heap[0];
    first_block->last_block = NULL;
    first_block->next_block = NULL;
    first_block->size = HEAP_SIZE - sizeof(mem_block_t);
    first_block->location = 0;
    first_block->is_used = false;
    first_block->block_id = block_counter++;
}

void* my_malloc(size_t size) {
    mem_block_t* existing_block = (mem_block_t*) &heap[0];

    do {
        if(!existing_block->is_used && existing_block->size >= size) {
            mem_block_t* next_block = (mem_block_t*) &heap[existing_block->location + sizeof(mem_block_t) + size];
            next_block->size = existing_block->size - sizeof(mem_block_t) - size;
            next_block->is_used = false;
            next_block->location = existing_block->location + sizeof(mem_block_t) + size;
            next_block->last_block = existing_block;
            next_block->next_block = existing_block->next_block;
            next_block->block_id = block_counter++;
    
            existing_block->next_block = next_block;
            existing_block->is_used = true;
            existing_block->size = size;
            return &heap[existing_block->location + sizeof(mem_block_t)];
        }

        existing_block = existing_block->next_block;
    } while(existing_block != NULL);


    return NULL;
}

void my_free(void* ptr) {
    // TODO trust user ptr or follow the linked list to see if it exists?
    mem_block_t* existing_block = (mem_block_t*) &heap[0];

    do {
        if(&heap[existing_block->location + sizeof(mem_block_t)] != ptr) {
            existing_block = existing_block->next_block;
            continue;
        }

        existing_block->is_used = false;

        for(int i = 0; i < existing_block->size; i++) {
            heap[existing_block->location + sizeof(mem_block_t) + i] = 0;
        }

        // combine this and previous block
        if(existing_block->last_block != NULL && existing_block->last_block->is_used == false) {
            mem_block_t* previous_block = existing_block->last_block;
            previous_block->next_block = existing_block->next_block;
            previous_block->size = previous_block->size + sizeof(mem_block_t) + existing_block->size;
            existing_block = previous_block;
        }

        if(existing_block->next_block != NULL && existing_block->next_block->is_used == false) {
            mem_block_t* next_block = existing_block->next_block;
            existing_block->size += sizeof(mem_block_t) + next_block->size;
            existing_block->next_block = next_block->next_block;
        }

        return;
    } while(existing_block != NULL);
}