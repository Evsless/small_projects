#include <string.h>
#include <string.h>
#include "malloc_o.h"

void test(void) {
    typedef struct foo
    {
        int a;
        int b;
    } foo_t;

    foo_t* foo = malloc_o(sizeof(foo_t));
    int* buzz = malloc_o(sizeof(int));
    char* bar = malloc_o(5);

    foo->a = 5;
    foo->b = 10;

    strcpy(bar, "bar");

    memcpy(buzz, &foo->a, sizeof(int));

    printf("Address: [%p], data: [%d] [%d]\n", foo, foo->a, foo->b);
	printf("Address: [%p], data: [%s] \n", bar, bar);
	printf("Address: [%p], data: [%d] \n", buzz, *buzz);

    free_o(foo);
    free_o(bar);
}

int main(int argc, char** argv) {
    test();
    printf("TEST: %d", sizeof(size_t*));
    return 0;
}