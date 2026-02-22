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

//#include "malloc.h"
#include "malloc.h"

t_malloc g_malloc = {NULL, NULL, NULL}; 


void show_alloc_mem()
{
    t_block *arr_blocks[3] = {g_malloc.tiny, g_malloc.small, g_malloc.large};
    for(int i = 0; i < 3; i ++)
    {
        if(arr_blocks[i])
        {
            // zone init
            printf("\n%s : %p [size = %.2f KB]\n", 
                i == 2 ? ("BIG") : (i == 0 ? "TINY": "SMALL"), 
                (void *)arr_blocks[i], 
                ((arr_blocks[i])->size * MIN_BLOCKS) / 1024.0
            );
            // each alloc
            t_block *b = arr_blocks[i];
            size_t acc = 0;
            while (b)
            {
                if (!b->free)
                {
                    void 
                        *start = (void *)(b + 1),
                        *end = (void *)((char *)(b + 1) + b->size);
                    acc += b->size;
                    printf("%p - %p : %zu bytes\n", start, end, b->size);
                }
                b = b->next;
            }
        }
    }
    printf("\n");
}



// void *mmap( void *addr, size_t length, int prot,
//             int flags, int fd, off_t offset);
static t_block  *allocate_block(size_t size)
{
    const size_t total_size = sizeof(t_block) + size;

    // t_block * just a *ptr 
    // to an adress that has n space available
    // if(!prev)
    // {
    //     if (!g_malloc.head)
    //         g_malloc.head = (BLOCK_);
    //     else
    //     {
    //         t_block *c = g_malloc.head;
    //         while (c->next)
    //             c = c->next;
    //         c->next = (BLOCK_);
    //     }
    // }else
    // {
    //     prev->next = BLOCK_;
    // }
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
    t_block *_current;
    if(type == BLOCK_TINY) 
        _current = g_malloc.tiny;
    else if (type == BLOCK_SMALL)
        _current = g_malloc.small;
    else
        _current = g_malloc.large;
    while (_current)
    {
        if (_current->free == 1 && _current->type == type 
            &&  _current->size >= size)
        {
            return _current;
        }
        _current = _current->next;
    }
    return NULL;
}





/*
    ZONE-TINY (multiple de page_size)
    ├─ [t_block][TINY_MAX]
    ├─ [t_block][TINY_MAX]
    ├─ [t_block][TINY_MAX]
    ├─ ...
    (100 fois min.)
*/
static void init_block_zone(int _type)
{
    if(_type != 1 && _type != 2)
        return ;

    // init small/medium zones
    // needed size for either TINY / SMALL () pour 100 bloc
    const size_t page_size = getpagesize(); // macOS ou Linux avec sysconf
    const size_t block_size = sizeof(t_block) + (
            (_type == 1) ? (TINY_MAX) : (SMALL_MAX)
        );
        
    // Une page est un bloc fixe de mémoire géré par le noyau.
    // Typiquement : 4096 bytes (4 KB) → le plus courant
    // arrondi a 4096 -> on most system  page size will be 4096.
    // page contains flags:
    // READ
    // WRITE
    // EXEC
    // NONE
    const size_t needed_size = block_size * MIN_BLOCKS;
    const size_t mmap_size = ((needed_size + page_size - 1) / page_size) * page_size;
    // mmap() alloction par tranche de 4096 bytes
    void *zone = mmap(NULL, mmap_size,
                      PROT_READ | PROT_WRITE,
                      MAP_ANON | MAP_PRIVATE,
                      -1, 0);
    if (zone == MAP_FAILED)
        return;
    if (_type == 1)
        g_malloc.tiny = (t_block *)((char *)zone);
    else
        g_malloc.small = (t_block *)((char *)zone);

    char *ptr = (char *)zone;
    t_block 
            *prev = NULL;
    const size_t  nb_blocks = mmap_size / (block_size);
    for (size_t i = 0; i < nb_blocks; i++)
    {
        t_block *block = (t_block *)ptr;

        block->size = (_type == 1) ? TINY_MAX : SMALL_MAX;
        block->type = (_type == 1) ? BLOCK_TINY : BLOCK_SMALL;
        block->next = NULL;
        block->free = 1;

        if (prev){
            prev->next = block;
        }
        prev = block;
        ptr += block_size;
    }
}








void    *malloc(size_t size)
{
    // printf("⚡️ YOURADRIEN-MALLOC (%zu) ⚡️\n", size);
    if(size == 0)
        size = 1;


    // blocks init
    if (!g_malloc.tiny && size <= TINY_MAX){ // [TINY]
        init_block_zone(1);
    }
    if (!g_malloc.small && size > TINY_MAX && size <= SMALL_MAX){ // [SMALL]
        init_block_zone(2);
    }

    // find bloc libre selon la taille
    t_block *block = NULL;
    if (size <= TINY_MAX) // tiny
    {
        block = find_free_block(size, BLOCK_TINY);
    } 
    else if (size <= SMALL_MAX) // small
    {
        block = find_free_block(size, BLOCK_SMALL);
    } 
    else { // large
        block = find_free_block(size, BLOCK_LARGE); // rare, pour réutiliser large déjà alloc
    }

    // bloc libre existe → réutiliser
    if (block) {
        block->free = 0;
        block->size = (size);
    } 
    // LARGE block
    // else if (size > SMALL_MAX) // create a new block (pour LARGE only)
    // { 
    //     block = allocate_block(size);
    //     if (!block)
    //         return NULL;
    //     block->type = BLOCK_LARGE;
    //     block->next = NULL;

    //     // push
    //     if (!g_malloc.head)
    //         g_malloc.head = (block);
    //     else
    //     {
    //         t_block *c = g_malloc.head;
    //         while (c->next)
    //             c = c->next;
    //         c->next = block;
    //     }
    // }
    // // SMALL / MEDIUM block aucuns libre found(zone pleine)
    // else {
    //     return NULL; 
    // }
    // return usable space + 1 after *ptr
    return (void *)(block + 1);
}


