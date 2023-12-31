#ifndef __STDMALLOC_H__
#define __STDMALLOC_H__

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <stdio.h>
#include <unistd.h>

/**********************************************************************************************************************
 *  MACRO
 *********************************************************************************************************************/
/**
 * STD_PAGE_SIZE
 * Page size is defined in POSIX
 */
#define STD_PAGE_SIZE sysconf(_SC_PAGE_SIZE)

/**
 * HEAP HEADER SIZE
 * Heap header size is equal to heap_t struct size.
 *     heap_t       - 8 byte;
 *     data_group_t - 4 byte;
 *     size_t       - 8 byte.
 */
#define HEAP_HEADER 48u

/**
 * BLOCK HEADER SIZE
 * Block header size is equal to block_t struct size.
 *     block_t        - 8 byte;
 *     size_t         - 8 byte;
 *     block_status_t - 4 byte;
 */
#define BLOCK_HEADER 40u

#define HEAP_SMALL_SIZE 4 * STD_PAGE_SIZE
#define HEAP_MEDIUM_SIZE 32 * STD_PAGE_SIZE

#define HEAP_SMALL_BLOCK_SIZE_THR (HEAP_SMALL_SIZE / 128)
#define HEAP_MEDIUM_BLOCK_SIZE_THR (HEAP_MEDIUM_SIZE / 128)

#define SHIFT_POINTER_LEFT(ptr, shift_by)  ((void *)ptr - shift_by)
#define SHIFT_POINTER_RIGHT(ptr, shift_by) ((void *)ptr + shift_by)

/**********************************************************************************************************************
 *  ENUM
 *********************************************************************************************************************/
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

typedef enum merge_direction
{
    PREV_BLOCK,
    NEXT_BLOCK
} merge_direction_t;

/**********************************************************************************************************************
 *  STRUCTS
 *********************************************************************************************************************/
typedef struct heap
{
    struct heap *next;
    struct heap *prev;
    data_group_t data_group;
    size_t total_capacity;
    size_t remaining_space;
    size_t block_count;
} heap_t;

typedef struct block
{
    struct heap *block_owner;
    struct block *next;
    struct block *prev;
    size_t data_size;
    block_status_t block_status;
} block_t;

/**********************************************************************************************************************
 * GLOBAL FUNCTION DECLARATION
 *********************************************************************************************************************/
void *stdmalloc(size_t size);
void *stdcalloc(size_t num, size_t size);
void *stdrealloc(void* ptr, size_t new_size);
void stdfree(void *ptr);

#endif /* __MALLOC_H */