#include <stdio.h>
#include "stdmalloc.h"

#define SHIFT_POINTER(ptr, shift_by) ((void*)ptr + shift_by)


int main(int argc, char** argv) 
{
    int* a;
    printf("test: %p\n", a);
    a = SHIFT_POINTER(a, 8);
    printf("test 2: %p\n", a);
    return 0;
}