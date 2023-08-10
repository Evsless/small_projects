#include "malloc_o.h"

#include <assert.h>

static entity_t LIST[40];
static uint16 IN_USE;

static void LOGGER(void) {
    printf("CURRENT_LIST:\n");
    for (uint16 i = 0; i < IN_USE; i++) {
        printf("Data + HEADER : [%p]. Free memory in section: [%u]\n", LIST[i].ptr, LIST[i].size);
    }
    printf("-----------------------------------------------------------------\n\n");
}

static entity_t* new_entity(size_t size) {
    if (LIST[0].ptr == NULL && LIST[0].size == 0) {
        static virtual_memory_t vm;
        LIST[0].ptr = vm.heap;
        LIST[0].size = HEAP_SIZE;
        IN_USE++;

        LOGGER();
    }

    entity_t* curr_best = LIST;

    for (uint16 i = 0; i < IN_USE; i++) {
        if (LIST[i].size >= size && LIST[i].size < curr_best->size) {
            curr_best = &LIST[i];
        }
    }

    return curr_best;
}

void* malloc_o(size_t size) {
    assert(size <= HEAP_SIZE);

    size += HEADER;

    entity_t* element = new_entity(size);

    uint8* start = element->ptr;
    uint8* user_start = element->ptr + HEADER;

    *start = size;

    element->ptr += size;
    element->size -= size;

    assert(element->size >= 0);

    LOGGER();
    return user_start;
}

void free_o(void* ptr) {
    uint8* start = (uint8*) ptr - HEADER;

    LIST[IN_USE].ptr = start;
    LIST[IN_USE].size = *start;

    IN_USE++;

    LOGGER();
}