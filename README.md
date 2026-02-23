# My Memory Allocator

I asked an LLM to suggest projects I could build in C to learn the language and learn more about embedded development. It suggested that I write my own memory allocator.

The LLM told me to use a char array as my heap and to satsify the following interface:

    #define HEAP_SIZE 1024
    static unsigned char heap[HEAP_SIZE];
    void my_allocator_init(void);
    void* my_malloc(size_t size);
    void my_free(void* ptr);

I went off on my own from there, googling some C syntax and with some distant memories of having read about memory allocators before (apparently there's an interesting one somewhere at the core of Quake?)

I assume it's a pretty terrible implementation, and I definitely do not recommend learning from it or using it for anything. I may build upon it or improve it in the future.

## Building

- Clone the project
- Use CMake to build I guess?

## Future plans

- Nothing concrete

