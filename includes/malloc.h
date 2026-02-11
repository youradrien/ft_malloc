
#ifndef MALLOC_H
#define MALLOC_H


#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>


// memory block
// → bloc1 [100 | free=0]  
//      → bloc2 [200 | free=0] 
//          → bloc3 [50 | free=0] 
//              → NULL
typedef struct s_block {
    size_t size;
    int free;
    struct s_block *next;
} t_block;

// allocation structure
typedef struct s_malloc {
    t_block *head;


    t_block *tiny;
    t_block *small;
    t_block *large;
} t_malloc;

// Global allocator (seule variable globale autorisée)
extern t_malloc g_malloc;

#endif
