#include "stdmalloc.h"

#define MALLOC_MEM_ALIGNMENT 0x08u

#define ALLIGN_BLOCK(size) (size + (MALLOC_MEM_ALIGNMENT - 1)) & (MALLOC_MEM_ALIGNMENT - 1);
#define SHIFT_POINTER(ptr, shift_by) ((void*)ptr + shift_by)

static heap_t* head;

static data_group_t getDataGroup(size_t size)
{
    data_group_t curr_data_group;

    if (HEAP_SMALL - HEAP_HEADER >= size)
    {
        curr_data_group = SMALL;
    }
    else if (HEAP_MEDIUM - HEAP_HEADER >= size) 
    {
        curr_data_group = MEDIUM;
    }
    else
    {
        curr_data_group = LARGE;
    }
    return data_group_t;
}

static void findAvailableBlock(size_t size, heap_t** heap, block_t block**)
{
    data_group_t data_group = getDataGroup(size);

    heap_t* heap;   /* PROBABLY SOME ACTIONS ON SIGNING TO IS THE HEADER OF THE HEAP */
    block_t* block;


}

static void* getAvailableLocation(size_t size) 
{
    heap_t* heap;
    block_t* block;
    
    for (heap_t* heap = head; heap != NULL; heap = heap->next)
    {
        if (heap->data_group != data_group)
            continue;

        for (block; block != NULL; block = block ->next)
        {
            if (block->data_size >= size)
                return (void*) block
        }
        if (heap->data_group == data_group && heap->remaining_space >= size)
    }
    
}

void* malloc(size_t size) 
{
    size = ALLIGN_BLOCK(size);
}