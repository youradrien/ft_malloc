
#ifndef MALLOC_H
#define MALLOC_H


#include <pthread.h>
#include <stddef.h>
// #include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_ALLOC (1024UL * 1024UL * 1024UL) // 1 Go / Bloc max éviter overflow

// small fragmentations
#define TINY_MAX 128 // small structs, strings, pointers
#define SMALL_MAX 2048 // medium buffers, parsing, I/O buffers

extern pthread_mutex_t	g_malloc_mutex;

// # define MIN_BLOCKS  128
# define MASK_0XFFF		(1 << 12) - 1
# define MALLOC_ZONE	(1 << 7) // 128
#define PAGE_MASK 0xFFF // 4096

typedef enum e_block_type {
    BLOCK_TINY,
    BLOCK_SMALL,
    BLOCK_LARGE
} t_block_type;


typedef struct s_block {
    size_t size;

    struct s_block *next;      // alloc list
    struct s_block *prev;      // alloc list
    struct s_block *next_free; // free list
} t_block;

// page
//  ├── free list
//  └── alloc list
typedef struct s_page {
    struct s_page *next;
    struct s_page *prev;

    t_block *free;
    t_block *alloc;
} t_page;

// allocation structure
typedef struct s_malloc {
    t_page *tiny;
    t_page *small;

    t_block *large;
} t_malloc;

// var global allocator (seule var-globale autorisée)
extern t_malloc g_malloc;

// func def
void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);

// utils
size_t   ft_align(size_t size, size_t mask);
size_t  page_size(size_t size);
void    show_alloc_mem();
bool	is_valid_block(const void *ptr);

#endif




