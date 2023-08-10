#ifndef __MALLOC_O__
#define __MALLOC_O__

#include <stdio.h>
#include <stdint.h>

#define uint8 uint8_t
#define uint16 uint16_t

#define STACK_SIZE 32
#define HEAP_SIZE STACK_SIZE * 4
#define HEADER 4

typedef struct virtual_memory
{
    uint8 stack[STACK_SIZE];
    char** unmapped;
    uint8 heap[HEAP_SIZE];

    struct
    {
        char** data;
        char** bss;
        char* txt;
    } data_t;
    
} virtual_memory_t;

typedef struct entity
{
    uint8* ptr;
    int size;
} entity_t;



void* malloc_o(size_t size);

void free_o(void* ptr);

#endif /* __MALLOC_O__ */