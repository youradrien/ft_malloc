#include <stdio.h>
#include <stddef.h>
#include "malloc.h" // ton malloc

int main(void)
{
    void *a = malloc(42);
    void *b = malloc(128);
    free(a);
    free(b);
    printf("✅ Test finished\n");
    return 0;
}