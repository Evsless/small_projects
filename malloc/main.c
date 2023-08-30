#include "stdmalloc.h"

int main(int argc, char *argv[])
{
    int* a = stdmalloc(16);
    int* b = stdmalloc(32);

    stdfree(a);
    stdfree(b);
    return 0;
}