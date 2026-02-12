
#ifndef MALLOC_H
#define MALLOC_H


#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define MAX_ALLOC (1024UL * 1024UL * 1024UL) // 1 Go / Bloc max éviter overflow
#define SMALL_MAX 128
#define MEDIUM_MAX 1024

typedef enum e_block_type {
    BLOCK_SMALL,
    BLOCK_MEDIUM,
    BLOCK_LARGE
} t_block_type;
// memory block
// → bloc1 [100 | free=0]  
//      → bloc2 [200 | free=0] 
//          → bloc3 [50 | free=0] 
//              → NULL
typedef struct s_block {
    size_t size;
    int free;
    struct s_block *next;
    t_block_type type;
} t_block;


// allocation structure
typedef struct s_malloc {
    t_block *head;

    t_block *small;
    t_block *medium;
} t_malloc;

// var global allocator (seule var-globale autorisée)
extern t_malloc g_malloc;

// func def
// void    *malloc(size_t size);
// void    free(void *ptr);
// void    *realloc(void *ptr, size_t size);
void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);

#endif
