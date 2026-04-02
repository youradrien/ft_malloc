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
inline size_t   ft_align(size_t size, size_t mask)
{
	return ((size + mask) & ~mask);
}


static inline void  show_alloc_tiny_small(t_page *page, const char *str)
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
    printf("Total : %lu bytes\n", total);
}

static inline void  show_alloc_large(void)
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
    printf("Total : %lu bytes\n", total);
}

void    show_alloc_mem()
{
	pthread_mutex_lock(&g_malloc_mutex);
	if (g_malloc.tiny)
		show_alloc_tiny_small(g_malloc.tiny, "TINY");
	if (g_malloc.small)
		show_alloc_tiny_small(g_malloc.small, "SMALL");
	if (g_malloc.large)
		show_alloc_large();
	pthread_mutex_unlock(&g_malloc_mutex);
}