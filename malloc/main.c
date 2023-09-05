#include "stdmalloc.h"

int main(int argc, char *argv[])
{
    int* a = stdmalloc(16);
    int* b = stdmalloc(600);
    int* c = stdmalloc(1300);

    stdfree(a);
    stdfree(b);
    stdfree(c);
    return 0;
}