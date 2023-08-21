
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/mman.h>

#include "stdmalloc.h"
#include "stdtypes.h"

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

#define ALLIGN_BLOCK(size) ((size + (MEM_ALIGNMENT - 1)) & ~(MEM_ALIGNMENT - 1))

#define DIFF_POINTER(lhs, rhs)       ((void *)lhs - (void *)rhs)
#define SHIFT_POINTER(ptr, shift_by) ((void *)ptr + shift_by)

/**********************************************************************************************************************
 * GLOBAL VARIABLES
 *********************************************************************************************************************/
static heap_t *head;

/**********************************************************************************************************************
 * LOCAL FUNCTION DECLARATION
 *********************************************************************************************************************/
static stdstatus_t findAvailableBlock(size_t size, block_t **block);
static stdstatus_t getAvailableLocation(size_t size, block_t **block_begin);

static size_t getHeapSize(size_t data_size);
static data_group_t getDataGroup(size_t size);

static stdstatus_t setupNewHeap(size_t size);
static void setupNewBlock(heap_t *heap_begin, size_t heap_size);

static void parseExistingBlock(block_t *block_begin, size_t size);

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

static void parseExistingBlock(block_t *block_begin, size_t size)
{
    /* Set retval to std status???*/
    block_t *block_end = SHIFT_POINTER(block_begin, BLOCK_HEADER + size);

    block_end->prev = block_begin;
    block_end->next = block_begin->next;

    block_begin->next = block_end;
    block_begin->data_size = size + BLOCK_HEADER;
    block_begin->block_status = OCCUPIED;

    block_end->block_status = FREE;
    block_end->data_size = DIFF_POINTER(block_end->next, block_end);

}

static void setupNewBlock(heap_t *heap_begin, size_t heap_size)
{
    block_t *block_begin = SHIFT_POINTER(heap_begin, HEAP_HEADER);

    block_begin->next = SHIFT_POINTER(heap_begin, heap_size);
    block_begin->data_size = DIFF_POINTER(block_begin->next, block_begin);
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

static stdstatus_t findAvailableBlock(size_t size, block_t **block_begin)
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
                it_heap->block_count++;

                *block_begin = it_block;
                retVal = STATUS_OK;
            }
        }
    }
    return retVal;
}

static stdstatus_t getAvailableLocation(size_t size, block_t **block_begin)
{
    stdstatus_t op_status = findAvailableBlock(size, block_begin);

    if (STATUS_OK == op_status)
    {
        /* The newly created block needs to be set up. The new block is basically is pointer to end of allocated block. */
        parseExistingBlock(*block_begin, size);
    }
    else
    {
        *block_begin = NULL;
    }
    return op_status;
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

    return block_begin;
}