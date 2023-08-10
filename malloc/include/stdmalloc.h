#ifndef __STDMALLOC_H__
#define __STDMALLOC_H__

#include <stdio.h>

#define PAGE_SIZE     4096   /* 4096 bytes = 16 kBytes*/

#define HEAP_HEADER  8
#define HEAP_SMALL   128
#define HEAP_MEDIUM  256


typedef enum data_group
{
    SMALL,
    MEDIUM,
    LARGE
} data_group_t;

typedef enum block_status
{
    OCCUPIED,
    FREE
} block_status_t;

typedef struct heap
{
    struct heap* next;
    struct heap* prev;
    data_group_t data_group;
    size_t total_capacity;
    size_t remaining_space;
    size_t heap_count;
} heap_t;

typedef struct block
{
    struct block* next;
    struct block* prev;
    size_t data_size;
    block_status_t block_status;
} block_t;

#endif /* __MALLOC_H */