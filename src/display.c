/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julessainthorant <marvin@42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 13:44:18 by julessainthor     #+#    #+#             */
/*   Updated: 2026/04/01 13:44:18 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"


// void show_alloc_mem()
// {
//     t_block *arr_blocks[3] = {g_malloc.tiny, g_malloc.small, g_malloc.large};
//     size_t total = 0;
//     for(int i = 0; i < 3; i ++)
//     {
//         if(arr_blocks[i])
//         {
//             // zone init
//             printf("\n%s : %p [zone size = %.2f KB]\n", 
//                 i == 2 ? ("LARGE") : (i == 0 ? "TINY": "SMALL"), 
//                 (void *)arr_blocks[i], 
//                 ((arr_blocks[i])->size * MIN_BLOCKS) / 1024.0
//             );  
//             // each alloc
//             t_block *b = arr_blocks[i];
//             size_t acc = 0;
//             while (b)
//             {
//                 if (b)
//                 {
//                     void 
//                         *start = (void *)(b + 1),
//                         *end = (void *)((char *)(b + 1) + b->size);
//                     acc += b->size;
//                     printf("%p - %p : %zu bytes\n", start, end, b->size);
//                 }
//                 b = b->next;
//             }
//             total += acc;
//             printf("- [%s] TOTAL : %zu bytes \n", 
//                 i == 2 ? ("LARGE") : (i == 0 ? "TINY": "SMALL"), 
//                 acc
//             );
//         }
//     }
//     printf("Total: %zu bytes\n", total);
//     printf("\n");
// }



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