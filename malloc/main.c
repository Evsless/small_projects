#include <stdio.h>
#include "stdmalloc.h"

#define SHIFT_POINTER(ptr, shift_by) ((void *)ptr + shift_by)

int main(int argc, char *argv[])
{
    void* test = stdmalloc(5);
    printf("TEST: %p\n", test);
    return 0;
}