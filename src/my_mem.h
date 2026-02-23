#ifndef MY_MEM_H
#define MY_MEM_H

#include <stddef.h>

/*
 * inititalize memory
 * must be called before `my_malloc` and/or `my_free`
 */
void my_allocator_init(void);

/*
 * allocate `req_size` bytes from the heap
 * @param req_size requested size for the new block
 * @return a pointer to the allocated memory, or `NULL` if insufficient memory is available
*/
void *my_malloc(size_t req_size);

/*
 * free bytes on the heap
 * @param ptr pointer to the memory to be freed
*/
void my_free(void *ptr);

/*
 * print information on the currently assigned blocks
*/
void print_blocks(void);

#endif
