#include <stdio.h>
#include "stdmalloc.h"

#define SHIFT_POINTER(ptr, shift_by) ((void*)ptr + shift_by)


int main(int argc, char* argv[])
{
    void* test = stdmalloc(4);

    if (NULL == test) {
        printf("NULL THERE\n");
    }
    return 0;
}