// #include <sys/mman.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
// #include "stdmalloc.h"

#define SHIFT_POINTER(ptr, shift_by) ((void *)ptr + shift_by)

#define ONEGB (1<<30)

int main(int argc, char *argv[])
{
    // int *a = (int *)stdmalloc(4);

    struct rlimit test;
    test.rlim_cur = 4096 * 44;
    int b = setrlimit(RLIMIT_DATA, &test);

    b = getrlimit(RLIMIT_DATA, &test);
    printf("CURRENT LIMIT: %lu\n", test.rlim_cur);

    printf("ALOCATING: %lu\n", test.rlim_cur);
    int* a = malloc(5);
    a[0] = 1;


    return 0;
}