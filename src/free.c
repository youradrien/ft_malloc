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

void free(void *ptr)
{
    if (!ptr)
        return;

    const t_block *ptr_block = (t_block *)ptr - 1;

    // retirer de la liste
    if (g_malloc.head == ptr_block)
        g_malloc.head = ptr_block->next;
    else {
        t_block *prev = g_malloc.head;
        while (prev->next && prev->next != ptr_block)
            prev = prev->next;
        if (prev->next == ptr_block)
            prev->next = ptr_block->next;
    }

    // munmap free memory
    munmap((t_block *)(ptr_block), sizeof(t_block) + (ptr_block)->size);
}
