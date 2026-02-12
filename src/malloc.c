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

#include "malloc.h"

t_malloc g_malloc = {NULL, NULL, NULL}; 

// void *mmap( void *addr, size_t length, int prot,
//             int flags, int fd, off_t offset);
static t_block  *allocate_block(size_t size)
{
    const size_t total_size = sizeof(t_block) + size;

    // t_block * is just a *ptr 
    // to an adress that has n space available
    t_block *block = mmap(NULL, total_size, 
                            PROT_READ | PROT_WRITE,
                            MAP_ANON | MAP_PRIVATE, 
                            -1, 0);
    if (block == MAP_FAILED){
        return NULL;
    }
    block->size = size;
    block->free = 0;
    block->next = NULL;
    return (block);
}



// find 1st free block present in da list
// head → block → block → block → NULL
// or 
// head → block → NULL → block → block
static t_block  *find_free_block(size_t size, t_block_type type)
{
    t_block *_current = g_malloc.head;
    while (_current)
    {
        if (_current->free && _current->type == type 
            &&  _current->size >= size)
        {
            return _current;
        }
        _current = _current->next;
    }
    return NULL;
}



void    *malloc(size_t size)
{
    if (size == 0 || size > MAX_ALLOC){
        return NULL;
    }

    // 1st malloc only 
    // init small/medium zones
    const size_t page_size = getpagesize(); // macOS ou Linux avec sysconf
    if (!g_malloc.small && size <= SMALL_MAX) // [SMALL]
    {
        const size_t small_sz = (16 * page_size);  // 16 pages pour SMALL
        g_malloc.small = mmap(NULL, small_sz,
                               PROT_READ | PROT_WRITE,
                               MAP_ANON | MAP_PRIVATE,
                               -1, 0);

        t_block *first = (t_block *) g_malloc.small;
        first->size = small_sz - sizeof(t_block);
        first->free = 1;
        first->type = BLOCK_SMALL;
        first->next = NULL;
        g_malloc.head = (first); // push into list
    }
    if (!g_malloc.medium && size <= MEDIUM_MAX) // [MEDIUM]
    {
        const size_t medium_sz = (128 * page_size); // 128 pages pour MEDIUM
        g_malloc.medium = mmap(NULL, medium_sz,
                            PROT_READ | PROT_WRITE,
                            MAP_ANON | MAP_PRIVATE,
                            -1, 0);

        t_block *second = (t_block *) g_malloc.medium;
        second->size = medium_sz - sizeof(t_block);
        second->free = 1;
        second->type = BLOCK_MEDIUM;
        second->next = NULL;
        // push 2nd to the list
        t_block *c = g_malloc.head;
        while (c->next)
            c = c->next;
        c->next = (second);
    }

    t_block *block = NULL;
    
    //  bloc libre selon la taille
    if (size <= SMALL_MAX) // small
    {
        block = find_free_block(BLOCK_SMALL, size);
    } 
    else if (size <= MEDIUM_MAX) // medium
    {
        block = find_free_block(BLOCK_MEDIUM, size);
    } 
    else { // large
        block = find_free_block(BLOCK_LARGE, size); // rare, pour réutiliser large déjà alloc
    }


    // bloc libre existe → réutiliser
    if (block) {
        block->free = 0; 
    } 
    // créer un nouveau bloc (seulement pour LARGE)
    else if (size > MEDIUM_MAX) // create a new block (pour LARGE only)
    { 
        block = allocate_block(size);
        if (!block)
            return NULL;
        block->type = BLOCK_LARGE;
        block->next = NULL;

        // push
        if (!g_malloc.head) {
            g_malloc.head = block;
        } else {
            t_block *c = g_malloc.head;
            while (c->next)
                c = c->next;
            c->next = block;
        }
    }
    // SMALL / MEDIUM → aucun bloc libre found → return NULL (zone pleine)
    else {
        return NULL; 
    }
    // return usable space + 1 after *ptr
    return (void *)(block + 1);
}

