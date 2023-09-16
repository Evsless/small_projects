#include "stdmalloc.h"
#include "stdstring.h"

int main(int argc, char *argv[])
{
    int *a = stdcalloc(4, sizeof(int));
    int *b = stdmalloc(8);
    a = stdrealloc(a, 4);
    stdfree(a);
    return 0;
}