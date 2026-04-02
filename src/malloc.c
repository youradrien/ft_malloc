/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julessainthorant <marvin@42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 17:23:44 by julessainthor     #+#    #+#             */
/*   Updated: 2026/02/11 17:23:47 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "malloc.h"
#include "malloc.h"

t_malloc g_malloc = {0, 0, 0};
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;


// create a block -> move from free list to alloc list
static inline void *block_create(t_block **free, t_block **alloc, const size_t size)
{
    t_block *b = *free; // head from free
    if (!b)
        return NULL;

    // remove from free list
    *free = b->next;
    if (*free)
        (*free)->prev = NULL;

    // push to alloc list : head insertion O(1) complexity
    if (*alloc)
        (*alloc)->prev = (b);

    b->next = *alloc;
    b->prev = NULL;
    b->size = size;

    *alloc = (b);
    return (b + 1); // memory usable by user
}




//initialize a new page: create free blocks inside page
static inline void mem_init_zone(t_page **tiny_small_page, t_page *p, const size_t zone_size)
{
    // insert page into liste (double linked)
    // p <-> page1 <-> page2 <-> page3
    p->prev = NULL;
    if ((p->next = *tiny_small_page))
        p->next->prev = p;
    *tiny_small_page = p;

    // init block lists
    t_block *free_block = (void *)p + sizeof(t_page);
    p->alloc = NULL;
    p->free = free_block; // start free here

    // make blocks -> [block][data][block][data][block][data]
    size_t total_size = zone_size * (MALLOC_ZONE);
    while ((void *)free_block + sizeof(t_block) + zone_size <= (void *)p + total_size)
    {
        free_block->next = (void *)free_block + sizeof(t_block) + zone_size;
        free_block->next->prev = free_block;
        free_block = free_block->next;
    }
    free_block->next = NULL;
}




// tiny/small malloc: find a free block or mmap a new page
static inline void  *malloc_tiny_small(t_page **tiny_small_page, const size_t zone_size, const size_t size)
{
    t_page *p = *tiny_small_page;

    // find a page with free blocks
    while (p && !p->free)
        p = p->next;

    if (!p)
    {
        // mmap new page()
        p = mmap(NULL, zone_size * (MALLOC_ZONE), PROT_READ | PROT_WRITE,
                   MAP_ANON | MAP_PRIVATE, -1, 0);
        if (p == MAP_FAILED)
            return NULL;
        
        mem_init_zone(tiny_small_page, p, zone_size);
    }

    return block_create(&p->free, &p->alloc, ft_align(size, 31));
}




// large malloc: mmap directly
static void *malloc_large(size_t size)
{
    size_t page_size = getpagesize();
    size_t total = ft_align(size + sizeof(t_block), page_size - 1) * 100;

    t_block *b = mmap(NULL, total, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (b == MAP_FAILED)
        return NULL;

    b->size = ft_align(size, 31);
    b->prev = NULL;
    if ((b->next = g_malloc.large))
        g_malloc.large->prev = b;
    g_malloc.large = (b);

    return (b + 1);
}


void    *malloc(size_t size)
{
    if (size == 0)
        return NULL;

    size_t type = (size > TINY_MAX) + (size > SMALL_MAX);
    void *ptr = NULL;

    pthread_mutex_lock(&g_malloc_mutex);
    if (type == 0)
        ptr = malloc_tiny_small(&g_malloc.tiny, TINY_MAX, size);
    else if (type == 1)
        ptr = malloc_tiny_small(&g_malloc.small, SMALL_MAX, size);
    else
        ptr = malloc_large(size);
    pthread_mutex_unlock(&g_malloc_mutex);

    return ptr;
}