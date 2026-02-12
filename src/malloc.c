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



static void init_block_zone(int _type)
{
    // init small/medium zones
    const size_t page_size = getpagesize(); // macOS ou Linux avec sysconf

    size_t mmap_size;
    if(_type == 1) // tiny
        mmap_size = (16 * page_size);  // 16 pages pour SMALL
    if(_type == 2) // small
        mmap_size = (128 * page_size); // 128 pages pour MEDIUM
    g_malloc.small = mmap(NULL, (mmap_size),
                            PROT_READ | PROT_WRITE,
                            MAP_ANON | MAP_PRIVATE,
                            -1, 0);

    t_block *BLOCK_= (t_block *) g_malloc.small;
    BLOCK_->size = mmap_size - sizeof(t_block);
    BLOCK_->free = 1;
    BLOCK_->type = _type == 1 ? BLOCK_TINY : BLOCK_SMALL;
    BLOCK_->next = NULL;

    if (!g_malloc.head)
        g_malloc.head = (BLOCK_);
    else
    {
        t_block *c = g_malloc.head;
        while (c->next)
            c = c->next;
        c->next = (BLOCK_);
    }

}



void    *malloc(size_t size)
{
    if (size == 0 || size > MAX_ALLOC){
        return NULL;
    }

    // 1st malloc only 
    if (!g_malloc.tiny && size <= SMALL_MAX) // [TINY]
    {
        init_block_zone(1);
    }
    if (!g_malloc.small && size <= MEDIUM_MAX) // [SMALL]
    {
        init_block_zone(2);
    }

    t_block *block = NULL;
    
    //  bloc libre selon la taille
    if (size <= SMALL_MAX) // tiny
    {
        block = find_free_block(BLOCK_TINY, size);
    } 
    else if (size <= MEDIUM_MAX) // small
    {
        block = find_free_block(BLOCK_SMALL, size);
    } 
    else { // large
        block = find_free_block(BLOCK_LARGE, size); // rare, pour réutiliser large déjà alloc
    }


    // bloc libre existe → réutiliser
    if (block) {
        block->free = 0; 
    } 
    // LARGE block
    else if (size > MEDIUM_MAX) // create a new block (pour LARGE only)
    { 
        block = allocate_block(size);
        if (!block)
            return NULL;
        block->type = BLOCK_LARGE;
        block->next = NULL;

        // push
        if (!g_malloc.head)
            g_malloc.head = (block);
        else
        {
            t_block *c = g_malloc.head;
            while (c->next)
                c = c->next;
            c->next = block;
        }
    }
    // SMALL / MEDIUM block aucuns libre found(zone pleine)
    else {
        return NULL; 
    }
    // return usable space + 1 after *ptr
    return (void *)(block + 1);
}

