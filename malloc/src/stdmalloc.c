#include "stdmalloc.h"
#include "stdtypes.h"

#define MALLOC_MEM_ALIGNMENT 0x08u

#define ALLIGN_BLOCK(size) (size + (MALLOC_MEM_ALIGNMENT - 1)) & (MALLOC_MEM_ALIGNMENT - 1);
#define SHIFT_POINTER(ptr, shift_by) ((void *)ptr + shift_by)

static heap_t *head;

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
    return curr_data_group;
}

static void setupNewBlock(block_t *block_begin, size_t size)
{
    /* Set retval to std status???*/
    block_t *block_end = SHIFT_POINTER(block_begin, BLOCK_HEADER + size);

    block_end->prev = block_begin;
    block_end->next = block_begin->next;
    block_begin->next = block_end;

    block_begin->block_status = OCCUPIED;
    block_end->block_status = FREE;

    block_end->data_size = block_end->next - block_end;
}

static stdstatus_t findAvailableBlock(size_t size, heap_t **heap, block_t **block)
{
    stdstatus_t retVal = STATUS_NOT_OK;
    data_group_t data_group = getDataGroup(size);

    for (heap_t *it_heap = head; it_heap != NULL; it_heap = it_heap->next)
    {
        if (it_heap->data_group != data_group)
            continue;

        for (block_t *it_block = SHIFT_POINTER(it_heap, HEAP_HEADER); it_block != NULL; it_block = it_block->next)
        {
            if ((FREE == it_block->block_status) && (it_block->data_size >= size + BLOCK_HEADER))
            {
                *heap = it_heap;
                *block = it_block;
                retVal = STATUS_OK;
            }
        }
    }
    return retVal;
}

static void *getAvailableLocation(size_t size)
{
    void *block_location;

    heap_t *heap;
    block_t *block;

    stdstatus_t op_status = findAvailableBlock(size, &heap, &block);

    if (STATUS_OK == op_status)
    {
        /* Need to prepare a block */
        block_location = (void *)SHIFT_POINTER(block, size);
        /* The newly created block needs to be set up. The new block is basically is pointer to end of allocated block. */
        setupNewBlock(block, size);
    }
    else
    {
        block_location = NULL;
    }
    return block_location;
}

void *stdmalloc(size_t size)
{
    size = ALLIGN_BLOCK(size);

    void *block_start = getAvailableLocation(size);

    return block_start;
}