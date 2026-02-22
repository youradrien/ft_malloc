// main.c
#include <stdio.h>
#include "malloc.h"

int main() {
    // tiny
    void *result = malloc(12);
    (void)(result);
    void *a = malloc(24);
    (void)(a);
    // 
    void *medium = malloc(256);
    (void)(medium);

    show_alloc_mem();
    return 0;
}
