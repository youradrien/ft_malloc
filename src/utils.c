/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julessainthorant <marvin@42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 20:30:18 by julessainthor     #+#    #+#             */
/*   Updated: 2026/04/01 20:30:19 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "malloc.h"

size_t  page_size(size_t size)
{
	static const size_t	malloc_size[3] = {BLOCK_TINY, BLOCK_SMALL, BLOCK_LARGE};

	return (malloc_size[(size > TINY_MAX) + (size > SMALL_MAX)]);
}

// align memory on (mask + 1) bytes with (mask + 1) being a power of 2
// arrondir size au multiple supérieur de (mask + 1)
inline size_t   ft_align(size_t size, size_t mask)
{
	return ((size + mask) & ~mask);
}


static inline size_t  show_alloc_tiny_small(t_page *page, const char *str)
{
	t_block				*b;
	size_t				total;

	total = 0;
	while (page)
	{
		printf("%s : %p\n", str, page);
		b = page->alloc;
		while (b)
		{
			printf("%p - %p : %lu bytes\n", (void *)b+sizeof(t_block),  (void *)b+sizeof(t_block)+b->size,  b->size);
			total += b->size;
			b = b->next;
		}
		page = page->next;
	}
    return total;
}

static inline size_t  show_alloc_large(void)
{
	t_block			*b;
	size_t			total;

	total = 0;
	printf("LARGE : %p\n", g_malloc.large);
	b = g_malloc.large;
	while (b)
	{
		printf("%p - %p : %lu bytes\n", (void *)b+sizeof(t_block),  (void *)b+sizeof(t_block)+b->size,  b->size);
		total += b->size;
		b = b->next;
	}
    return total;
}

void    show_alloc_mem()
{
    size_t  total = 0;

    pthread_mutex_lock(&g_malloc_mutex);
	if (g_malloc.tiny)
		total += show_alloc_tiny_small(g_malloc.tiny, "TINY");
	if (g_malloc.small)
		total += show_alloc_tiny_small(g_malloc.small, "SMALL");
	if (g_malloc.large)
		total += show_alloc_large();
	pthread_mutex_unlock(&g_malloc_mutex);

    printf("Total : %lu bytes\n", total);
}


/*
** Go through list of block and check if we can identify a valid block address
** corresponding to the pointer sent. We add the header sizeof(t_block).
*/
// bool	is_valid_block(const void *ptr, size_t size)
// {
// 	void		*block_addr;
// 	t_block		*b;

// 	if (size > SMALL_MAX)
// 		b = g_malloc.large;
// 	else if (size > TINY_MAX)
// 		b = (t_block*)g_malloc.small + sizeof(t_page);
// 	else
// 		b = (t_block*)g_malloc.tiny + sizeof(t_page);
// 	block_addr = (b) + sizeof(t_block);
// 	while (b && block_addr < ptr)
// 	{
// 		if (block_addr == ptr)
// 			return (true);
// 		b = b->next;
// 		block_addr = b + sizeof(t_block);
// 	}
// 	return (false);
// }
bool is_valid_block(const void *ptr)
{
    t_block *b;

    // LARGE
    b = g_malloc.large;
    while (b)
    {
        if ((void *)(b + 1) == ptr)
            return true;
        b = b->next;
    }
    // SMALL
    t_page *p = g_malloc.small;
    while (p)
    {
        b = (t_block *)((char *)p + sizeof(t_page));
        while (b)
        {
            if ((void *)(b + 1) == ptr)
                return true;
            b = b->next;
        }
        p = p->next;
    }
    // TINY
    p = g_malloc.tiny;
    while (p)
    {
        b = (t_block *)((char *)p + sizeof(t_page));
        while (b)
        {
            if ((void *)(b + 1) == ptr)
                return true;
            b = b->next;
        }
        p = p->next;
    }
    return false;
}