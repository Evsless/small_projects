/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/mman.h>

#include "stdmalloc.h"
#include "stdtypes.h"

#ifndef RELEASE
#include "logger.h"
#endif

/**********************************************************************************************************************
 *  MACRO
 *********************************************************************************************************************/

/* Memory alignment differs depending on OS.
   #. Windows 32bit : 4  byte alignment;
   #. Windows 64bit : 8  byte alignment;
   #. Linux (GNU)   : 8  byte alignment;
   #. MacOS         : 16 byte alignment.
*/
#define MEM_ALIGNMENT 0x08u
#define EMPTY 0

#define ALLIGN_BLOCK(size) ((size + (MEM_ALIGNMENT - 1)) & ~(MEM_ALIGNMENT - 1))

/**********************************************************************************************************************
 * GLOBAL VARIABLES
 *********************************************************************************************************************/
static heap_t *head;

/**********************************************************************************************************************
 * LOCAL FUNCTION DECLARATION
 *********************************************************************************************************************/
static stdstatus_t findAvailableBlock(size_t size, heap_t **heap, block_t **block);
static stdstatus_t getAvailableLocation(size_t size, block_t **block_begin);

static size_t getHeapSize(size_t data_size);
static data_group_t getDataGroup(size_t size);

static stdstatus_t setupNewHeap(size_t size);
static void setupNewBlock(heap_t *heap_begin, size_t heap_size);

static void parseExistingBlock(heap_t *heap, block_t *block_begin, size_t size);
static boolean_t mergeBlocks(block_t **block_lhs, block_t **block_rhs, merge_direction_t direction);

/**********************************************************************************************************************
 * LOCAL FUNCTION DEFINITION
 *********************************************************************************************************************/
static data_group_t getDataGroup(size_t size)
{
    data_group_t curr_data_group;

    if (HEAP_SMALL_BLOCK_SIZE_THR - HEAP_HEADER >= size)
    {
        curr_data_group = SMALL;
    }
    else if (HEAP_MEDIUM_BLOCK_SIZE_THR - HEAP_HEADER >= size)
    {
        curr_data_group = MEDIUM;
    }
    else
    {
        curr_data_group = LARGE;
    }
    return curr_data_group;
}

static size_t getHeapSize(size_t data_size)
{
    size_t heap_size = 0;
    data_group_t data_group = getDataGroup(data_size);

    if (SMALL == data_group)
    {
        heap_size = HEAP_SMALL_SIZE;
    }
    else if (MEDIUM == data_group)
    {
        heap_size = HEAP_MEDIUM_SIZE;
    }
    else
    {
        heap_size = data_size + HEAP_HEADER + BLOCK_HEADER;
    }

    return heap_size;
}

static void parseExistingBlock(heap_t *heap, block_t *block_begin, size_t size)
{
    block_t *block_end = SHIFT_POINTER(block_begin, BLOCK_HEADER + size);

    block_end->data_size = block_begin->data_size - size - BLOCK_HEADER;
    if (EMPTY != block_end->data_size)
    {
        block_end->prev = block_begin;
        block_end->next = block_begin->next;
        block_end->block_status = FREE;
    }
    else
    {
        block_end = NULL;
    }

    block_begin->next = block_end;
    block_begin->data_size = size + BLOCK_HEADER;
    block_begin->block_status = OCCUPIED;

    heap->remaining_space -= size + BLOCK_HEADER;
    if (EMPTY != heap->remaining_space)
        heap->block_count++;
}

static void setupNewBlock(heap_t *heap_begin, size_t heap_size)
{
    block_t *block_begin = SHIFT_POINTER(heap_begin, HEAP_HEADER);

    block_begin->data_size = heap_size - HEAP_HEADER;
    block_begin->block_status = FREE;

    heap_begin->block_count++;
}

static stdstatus_t setupNewHeap(size_t size)
{
    stdstatus_t allocation_status;
    size_t heap_size = getHeapSize(size);

    heap_t *new_heap = mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (NULL == new_heap)
    {
        allocation_status = STATUS_NOT_OK;
    }
    else
    {
        allocation_status = STATUS_OK;
        if (NULL != head)
        {
            new_heap->next = head;
            head->prev = new_heap;
        }
        new_heap->data_group = getDataGroup(size);
        new_heap->total_capacity = heap_size;
        new_heap->remaining_space = heap_size - HEAP_HEADER;

        setupNewBlock(new_heap, heap_size);

        head = new_heap;
    }
    return allocation_status;
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

static stdstatus_t getAvailableLocation(size_t size, block_t **block_begin)
{
    heap_t *heap;
    block_t *block;

    stdstatus_t op_status = findAvailableBlock(size, &heap, &block);

    if (STATUS_OK == op_status)
    {
        *block_begin = block;
        parseExistingBlock(heap, block, size);
    }
    else
    {
        *block_begin = NULL;
    }
    return op_status;
}

static boolean_t mergeBlocks(block_t **block_lhs, block_t **block_rhs, merge_direction_t direction)
{
    block_t *lhs_cpy = *block_lhs;
    block_t *rhs_cpy = *block_rhs;

    block_t *rhs_end = rhs_cpy->next;

    lhs_cpy->next = rhs_end;
    lhs_cpy->data_size += rhs_cpy->data_size;
    lhs_cpy->block_status = FREE;

    if (NULL != rhs_end)
        rhs_end->prev = lhs_cpy;

    if (PREV_BLOCK == direction)
        *block_rhs = lhs_cpy;

    rhs_cpy = NULL;

    return TRUE;
}

/**********************************************************************************************************************
 * GLOBAL FUNCTION DEFINITION
 *********************************************************************************************************************/
void *stdmalloc(size_t size)
{
    block_t *block_begin;
    size = ALLIGN_BLOCK(size);

    stdstatus_t op_status = getAvailableLocation(size, &block_begin);

    if (STATUS_NOT_OK == op_status)
    {
        op_status = setupNewHeap(size);

        op_status = getAvailableLocation(size, &block_begin);
    }

    block_begin = SHIFT_POINTER(block_begin, BLOCK_HEADER);

#ifndef RELEASE
    STDLOG(MALLOC, head);
#endif

    return block_begin;
}

void stdfree(void *ptr)
{
    block_t *block = ptr - BLOCK_HEADER;
    block_t *block_prev = block->prev;
    block_t *block_next = block->next;

    boolean_t merge_status = FALSE;

    if (NULL != block_prev && FREE == block_prev->block_status)
        merge_status = mergeBlocks(&block_prev, &block, PREV_BLOCK);

    if (NULL != block_next && FREE == block_next->block_status)
        merge_status = mergeBlocks(&block, &block_next, NEXT_BLOCK);

    if (TRUE != merge_status)
        block->block_status = FREE;

#ifndef RELEASE
    STDLOG(MALLOC, head);
#endif
}