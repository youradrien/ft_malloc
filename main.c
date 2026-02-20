// main.c
#include <stdio.h>
#include "malloc.h"

int main() {
    printf("----- MAIN ------ \n");
    void *result = malloc(3);
    (void)(result);
    return 0;
}
