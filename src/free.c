/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free  .c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julessainthorant <marvin@42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 19:23:44 by julessainthor     #+#    #+#             */
/*   Updated: 2026/02/11 19:23:47 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

// static void			free_unused_page(const int t, t_page *p)
// {
// 	if (p->prev)
// 		p->prev->next = p->next;
// 	else
// 	{
// 		if (t == BLOCK_TINY)
// 			g_malloc.tiny = p->next;
// 		else
// 			g_malloc.small = p->next;
// 	}
// 	if (p->next)
// 		p->next->prev = p->prev;
// 	munmap(p, p->total_size /* MALLOC_ZONE * (t == BLOCK_TINY ? TINY_MAX : SMALL_MAX) */);
// }

// // [tiny / small]
// static inline void	free_tiny_small(t_block *block, const int malloc_size, t_page *p)
// {
//     //1. remove block from page->alloc list
// 	if (block->prev)
// 		block->prev->next = block->next;
// 	else
// 		p->alloc = block->next;
// 	if (block->next)
// 		block->next->prev = block->prev;
//     //2. add block to page->free list
// 	block->prev = NULL;
// 	block->next = p->free;
// 	if (p->free)
// 		p->free->prev = block;
// 	p->free = block;
//     // printf("alloc list: %p\n", p->alloc);
// 	if (!p->alloc) // empty alloc page
//     {
//         // limit the munmap() calls w/ only unmaping empty pages
// 		free_unused_page(malloc_size, p);
//     }
// }

// static void			free_block(t_block *block)
// {
// 	const int		type = page_size(block->size);

// 	if (type == BLOCK_LARGE)
// 		free_large(block);
// 	else 
// 	{
// 		t_page *p = (type == BLOCK_TINY) ? g_malloc.tiny : g_malloc.small;
// 		while (! ((void *)block < (void *)p + MALLOC_ZONE * (type == 0 ? TINY_MAX : SMALL_MAX) 
//                 && (void *)block > (void *)p) 
//         )
// 			p = p->next;
// 		free_tiny_small(block, type, p);
// 	}
// }

// [large]
static inline void	free_large(t_block *block)
{
	const size_t	msize = ft_align(block->size + sizeof(t_block), MASK_0XFFF);

	if (block->prev)
		block->prev->next = block->next;
	else
		g_malloc.large = block->next;
	if (block->next)
		block->next->prev = block->prev;
	munmap(block, msize);
}



static inline void free_tiny_small(t_block *block, const int malloc_size, t_page *p)
{
    printf("block addr: %p\n", block);
    // remove block from alloc-list (-)
    if (block->prev)
        block->prev->next = block->next;
    else
        p->alloc = block->next;
    if (block->next)
        block->next->prev = block->prev;
    // add block to free-list (+)
    // block->prev = NULL;
    block->next = p->free;
    if (p->free)
        p->free->prev = block;
    p->free = block;
    // update alloc_count
    p->alloc_count--;
    (void)(malloc_size);
    
    // optional: only munmap if we really want to shrink memory
    //    on free_tiny_small() we do NOT munmap, PAGES stays in RAM
    // if (p->alloc_count == 0)
    //     free_unused_page(malloc_size, p);
}

void free(void *ptr)
{
    pthread_mutex_lock(&g_malloc_mutex);

    if (!ptr)
    {
        pthread_mutex_unlock(&g_malloc_mutex);
        return;
    }

    t_block *b = (t_block *)ptr - 1;

    if (!is_valid_block(ptr))
    {
        printf("INVALID free at %p\n", ptr);
        pthread_mutex_unlock(&g_malloc_mutex);
        return;
    }

    int type = page_size(b->size);

    if (type == BLOCK_LARGE)
    {
        free_large(b); // large mallocs: munmap
    }
    else
    {
        // find page containing the block
        t_page *p = (type == BLOCK_TINY) ? g_malloc.tiny : g_malloc.small;
        while (p)
        {
            void *start = (char *)p + sizeof(t_page);
            void *end = (char *)p + p->total_size;
            if ((void *)b >= start && (void *)b < end)
            {
                free_tiny_small(b, type, p);
                break;
            }
            p = p->next;
        }
    }
    pthread_mutex_unlock(&g_malloc_mutex);
}
