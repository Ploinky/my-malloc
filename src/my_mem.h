#ifndef MY_MEM_H
#define MY_MEM_H

#include <stddef.h>

// total memory size in bytes
#define HEAP_SIZE 1024

// heap memory
static unsigned char heap[HEAP_SIZE];

// inititalize memory
void my_allocator_init(void);

// allocate size bytes from the heap
void* my_malloc(size_t size);

// free bytes on the heap
void my_free(void* ptr);

// print information on the currently assigned blocks
void print_blocks(void);

#endif