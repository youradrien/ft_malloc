#!/bin/bash

# LIBRARY NAME
LIB="libft_malloc.so"

# Vérifie que la lib existe
if [ ! -f "$LIB" ]; then
    echo "❌ $LIB introuvable."
    echo "-> run 'make' pour créer $LIB."
    exit 1
fi

echo "===> LIB: $LIB existe "

# temp main.c
cat > main.c << EOF
#include <stdio.h>
// #include <string.h>
#include <stddef.h>   // <- pour size_t

#define TINY_SIZE   32
#define SMALL_SIZE  1024
#define LARGE_SIZE  1000000

// prototypes -> custom malloc
void *malloc(size_t size);
void free(void *ptr);

int main(void)
{    
    printf("\n\n");
    printf("--- TEST-TINY --- \\n");
    void *a = malloc(TINY_SIZE);
    printf("a: %d \n", a);
    /*
    void *tiny[150];
    for (int i = 0; i < 150; i++)
    {
        tiny[i] = malloc(TINY_SIZE);
        if (!tiny[i])
        {
            printf("Erreur malloc tiny\\n");
            return 1;
        }
        memset(tiny[i], 'A', TINY_SIZE);
    }

    printf("150 allocations TINY OK\\n");

    for (int i = 0; i < 150; i++)
        free(tiny[i]);

    printf("Free TINY OK\\n\\n");

    
    printf("=== TEST-SMALL ===\\n");
    void *small[150];
    for (int i = 0; i < 150; i++)
    {
        small[i] = malloc(SMALL_SIZE);
        if (!small[i])
        {
            printf("Erreur malloc small\\n");
            return 1;
        }
        memset(small[i], 'B', SMALL_SIZE);
    }
    printf("150 allocations SMALL OK\\n");
    for (int i = 0; i < 150; i++)
        free(small[i]);
    printf("Free SMALL OK\\n\\n");



    printf("=== TEST-LARGE ===\\n");
    void *large = malloc(LARGE_SIZE);
    if (!large)
    {
        printf("Erreur malloc large\\n");
        return 1;
    }
    memset(large, 'C', LARGE_SIZE);
    printf("Allocation LARGE OK\\n");
    free(large);
    printf("Free LARGE OK\\n\\n");


    printf("=== TEST REUTILISATION ===\\n");
    void *a = malloc(64);
    free(a);
    void *b = malloc(64);
    if (a == b)
        printf("Bloc réutilisé ✅\\n");
    else
        printf("Bloc différent (possible si pas de coalescing)\\n");
    free(b);
    
    printf("\\nTous les tests terminés.\\n");
    */
    return 0;
}
EOF

# Compile le main

echo "===> main.c: test_executable crée."

gcc main.c -Wall -Wextra -o test_exec
if [ $? -ne 0 ]; then
    echo "❌ Erreur compilation."
    exit 1
fi
echo "shyt compiled."

# Injecte ton malloc
# ----------------------
# Lancement avec ta libft_malloc.so
# ----------------------
#echo "===> Injection de ton malloc: ./libft_malloc.so"
#DYLD_INSERT_LIBRARIES=./libft_malloc_arm64_Darwin.so DYLD_FORCE_FLAT_NAMESPACE=1 ./test_exec
echo "===> Injection de ton malloc: ./libft_malloc.so"
# DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1 
./test_exec


rm -f test_exec main.c
