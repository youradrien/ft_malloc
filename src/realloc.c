/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julessainthorant <marvin@42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 21:23:44 by julessainthor     #+#    #+#             */
/*   Updated: 2026/02/11 21:23:47 by julessainthor    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "malloc.h"
#include <string.h> // pour memcpy


// 1. ptr == NULL → se comporte comme malloc(size)
// 2. size == 0 → se comporte comme free(ptr) et retourne NULL
// 3. essaye de changer la taille du bloc existant :
//     if [bloc] actuel est assez grand :
//           → on garde et on retourne ptr
//     else :
//           →  alloue new block de la nv size
//           →  copie content existant (jusqu’à min(old_size, new_size))
//            → free l’ancien bloc (free(*ptr))
//            → return new block

void    *realloc(void *ptr, size_t size)
{
    if (!ptr){
        return malloc(size);  // realloc(NULL, size) = malloc
    }

    if (size == 0) {
        free(ptr);           // realloc(ptr, 0) = free
        return NULL;
    }

    t_block *block = (t_block *)ptr - 1;

    // [block] déjà assez grand
    // on ne touche a rien: reduire la size du block 
    //    créerai un memory leak (sans split, création d'un block entre les 2)
    if (block->size >= size){
        return (ptr);
    }

    //  allocation [block] de la nouvelle [size]
    void *new_ptr = malloc((size));
    if (!new_ptr) {
        return NULL;
    }

    // [FROM SUBJECT] copies as much of the old data pointed to by "*ptr"
    const size_t copy_size = (block->size < size) ? 
            (block->size) : (size);
    memcpy(new_ptr, ptr, copy_size);

    free(ptr);
    return (new_ptr);
}
