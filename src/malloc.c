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

#include "malloc.h"
#include <unistd.h>
#include <string.h>

t_malloc g_malloc = {NULL};  // la seule variable globale



// void *mmap( void *addr, size_t length, int prot,
//             int flags, int fd, off_t offset);
static t_block  *allocate_block(size_t size)
{
    const size_t total_size = sizeof(t_block) + size;

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
static t_block  *find_free_block(size_t size)
{
    t_block *_current = g_malloc.head;
    while (_current)
    {
        if (_current->free && _current->size >= size){
            return _current;
        }
        _current = _current->next;
    }
    return NULL;
}




void    *malloc(size_t size)
{
    if (size == 0){
        return NULL;
    }
    t_block *block = find_free_block(size);

    if (block) { // re-use alr existing block
        block->free = 0; 
    } else { // create a new block 
        block = allocate_block(size);
        if (!block)
            return NULL;

        // straight head block
        if (!g_malloc.head){
            g_malloc.head = block;
        } 
        // or push it to the end
        else {
            t_block *c = g_malloc.head;
            while (c->next)
                c = c->next;
            c->next = block;
        }
    }
    // return usable space + 1 after *ptr
    return (void*)(block + 1);
}

