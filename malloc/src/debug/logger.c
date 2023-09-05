
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <stdio.h>
#include <stdarg.h>

#include "logger.h"
#include "stdmalloc.h"

/**********************************************************************************************************************
 *  MACRO
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * GLOBAL VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL FUNCTION DECLARATION
 *********************************************************************************************************************/
static void log_malloc(heap_t *heap);

/**********************************************************************************************************************
 * LOCAL FUNCTION DEFINITION
 *********************************************************************************************************************/
static void log_malloc(heap_t *head)
{
    size_t heap_cnt = 0;
    size_t block_cnt = 0;

    printf("INFO: MEMORY ALLOCATION OPERATION DETECTED\n\n");
    printf("---- MALLOC LOG BEGIN ----\n\n");
    for (heap_t *it_heap = head; it_heap != NULL; it_heap = it_heap->next)
    {
        printf("HEAP: %d | CAPACITY: %d | FREE SPACE %d | BLOCK AMOUNT %d\n\n",
               ++heap_cnt, it_heap->total_capacity, it_heap->remaining_space, it_heap->block_count);
        // printf("* HEAP NO       : %d\n", ++heap_cnt);
        // printf("* TOTAL CAPACITY: %d\n", it_heap->total_capacity);
        // printf("* FREE SPACE    : %d\n", it_heap->remaining_space);
        // printf("* NUM OF BLOCKS : %d\n\n", it_heap->block_count);

        block_cnt = 0;
        for (block_t *it_block = SHIFT_POINTER_RIGHT(it_heap, HEAP_HEADER); it_block != NULL; it_block = it_block->next)
        {
            printf(".) BLOCK NO    : %d\n", ++block_cnt);
            printf(".) BLOCK SIZE  : %ld\n", it_block->data_size);
            printf(".) BLOCK STATUS: %s\n\n", it_block->block_status == FREE ? "BLOCK_FREE" : "BLOCK_OCCUPIED");
        }
    }
    printf("---- MALLOC LOG END ----\n\n\n");
}

static void log_malloc_heap(heap_t *head)
{
    size_t heap_cnt = 0;

    printf("INFO: HEAP OPERATION DETECTED\n");
    printf("---- HEAP LOG BEGIN ----\n\n");

    for (heap_t *it_heap = head; it_heap != NULL; it_heap = it_heap->next)
    {
        printf("HEAP %d|\tBEGIN: %p|\tEND:%p\n", ++heap_cnt, it_heap, SHIFT_POINTER_RIGHT(it_heap, it_heap->total_capacity));
    }

    if (NULL == head)
        printf("NO MEMORY ALOCATED\n");

    printf("\n---- HEAP LOG END ----\n\n\n");
}

/**********************************************************************************************************************
 * GLOBAL FUNCTION DEFINITION
 *********************************************************************************************************************/
void log(component_t component, ...)
{
    va_list args_ptr;

    if (MALLOC_ALL == component)
    {
        va_start(args_ptr, 1);
        log_malloc(va_arg(args_ptr, heap_t *));
    }
    else if (MALLOC_HEAP == component)
    {
        va_start(args_ptr, 1);
        log_malloc_heap(va_arg(args_ptr, heap_t *));
    }
    else
    {
        /* NOTHING TO BE DONE */
    }
}