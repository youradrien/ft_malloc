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

/*
    TINY / SMALL = pools → réutilisation → fragmentation gérée
    LARGE = allocation directe → jetable
*/
void free(void *ptr)
{
    if (!ptr)
        return;

    t_block *block = (t_block *)ptr - 1;

    // double free protection
    if (block->free)
        return;

    if (block->type == BLOCK_LARGE)
    {
        // retirer de la liste
        if (g_malloc.large == block)
            g_malloc.large = block->next;
        else {
            t_block *head = g_malloc.large;
            while (head->next && head->next != block){
                head = head->next;
            }
            if (head->next == block)
                head->next = block->next;
        }
        munmap(block, sizeof(t_block) + block->size);
        return;
    }else{
        block->free = 1;
    }


}
