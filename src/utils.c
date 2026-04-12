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
	size_t total = 0;

	while (page)
	{
		printf("%s : %p\n", str, page);
		t_block *b = page->alloc;
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
	size_t			total = 0;

	printf("LARGE : %p\n", g_malloc.large);
	t_block			*b = g_malloc.large;
	while (b)
	{
		printf("%p - %p : %lu bytes\n", (void *)b+sizeof(t_block),  (void *)b+sizeof(t_block)+b->size,  b->size);
		total += b->size;
		b = b->next;
	}
    return total;
}

void    show_alloc_mem(void)
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


// go through each page, each page->alloc
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
    // SMALL/TINY
    for(int i = 0; i < 2; i ++)
    {
        t_page *p_arr[2] = {g_malloc.small, g_malloc.tiny};
        t_page *p = p_arr[i];
        while (p)
        {
            b = p->alloc;
            while (b)
            {
                if ((void *)(b + 1) == ptr)
                    return true;
                b = b->next;
            }
            p = p->next;
        }
    }
    return false;
}