#include "my_mem.h"

#include <stdio.h>

int main(void)
{
        my_allocator_init();

        print_blocks();

        int* x = my_malloc(sizeof(x));
        print_blocks();
        
        int* y = my_malloc(sizeof(y));
        print_blocks();
        
        printf("x: %d, y: %d\r\n", *x, *y);
        *x = 15;
        *y = 32;
        printf("x: %d, y: %d\r\n", *x, *y);

        my_free(x);
        print_blocks();

        printf("x: %d, y: %d\r\n", *x, *y);
        
        my_free(y);
        print_blocks();
        printf("x: %d, y: %d\r\n", *x, *y);
        return 0;
}