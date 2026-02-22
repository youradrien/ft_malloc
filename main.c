// main.c
#include <stdio.h>
#include "malloc.h"

int main() {
    printf("=== ALLOC ===\n");

    // --- TINY ---
    void *t1 = malloc(12);
    void *t2 = malloc(24);
    void *t3 = malloc(64);

    // --- SMALL ---
    void *s1 = malloc(256);

    // --- LARGE ---
    void *l1 = malloc(5000);

    (void)t1;
    (void)t2;
    (void)t3;
    (void)s1;
    (void)l1;

    show_alloc_mem();

    printf("\n=== FREE t2 ===\n");
    free(t2);

    show_alloc_mem();

    printf("\n=== REALLOC LIKE TEST ===\n");
    void *d = malloc(24); // devrait réutiliser b

    show_alloc_mem();

    printf("\n=== FREE ALL ===\n");
    free(t1);
    free(t2);
    free(t3);

    show_alloc_mem();
    return 0;
}
