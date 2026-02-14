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



// void *mmap( void *addr, size_t length, int prot,
//             int flags, int fd, off_t offset);
static t_block  *allocate_block(size_t size)
{
    const size_t total_size = sizeof(t_block) + size;

    // t_block * is just a *ptr 
    // to an adress that has n space available
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
    t_block *_current = g_malloc.head;
    while (_current)
    {
        if (_current->free && _current->type == type 
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
    const size_t page_size = getpagesize(); // macOS ou Linux avec sysconf
    const size_t block_size = sizeof(t_block) + (
            (_type == BLOCK_TINY) ? (TINY_MAX) : (SMALL_MAX)
        );
        
    // needed size for either TINY / SMALL () pour 100 blocs
    const size_t needed_size = block_size * MIN_BLOCKS;
    printf("-> needed size: %zu \n", needed_size);
    // arrondi a 4096 -> 
    //           on most system  page size will be 4096.
    //           this means every page will contain exactly 4,096 bytes of data
    const size_t mmap_size = ((needed_size + page_size - 1) / page_size) * page_size;
    printf("-> mmap() nbr: %zu \n", mmap_size);
    // mmap() alloction par tranche de 4096 bytes
    void *zone = mmap(NULL, mmap_size,
                      PROT_READ | PROT_WRITE,
                      MAP_ANON | MAP_PRIVATE,
                      -1, 0);
    if (zone == MAP_FAILED)
        return;

    if (_type == 1)
        g_malloc.tiny = (zone);
    else if (_type == 2)
        g_malloc.small = (zone);

    // découpage en blocs
    char *ptr = (char *)zone;
    t_block *prev = NULL;
    const size_t  nb_blocks = mmap_size / (block_size);
    for (size_t i = 0; i < nb_blocks; i++)
    {
        // t_block *BLOCK_= (t_block *) (
        //     _type == 1 ? g_malloc.tiny : g_malloc.small
        // );
        t_block *BLOCK_ = (t_block *)ptr;

        BLOCK_->size = mmap_size - sizeof(t_block);
        BLOCK_->free = 1;
        BLOCK_->type = _type == 1 ? BLOCK_TINY : BLOCK_SMALL;
        BLOCK_->next = NULL;

        if(!prev)
        {
            if (!g_malloc.head)
                g_malloc.head = (BLOCK_);
            else
            {
                t_block *c = g_malloc.head;
                while (c->next)
                    c = c->next;
                c->next = (BLOCK_);
            }
        }else
        {
            prev->next = BLOCK_;
        }

        prev = BLOCK_;
        ptr += block_size;
    }
}







void    *malloc(size_t size)
{
        FILE *f = fopen("./tmp/malloc.log", "a");
    if (f) {
        fprintf(f, "malloc called: %zu\n", size);
        fclose(f);
    }
    // setbuf(stdout, NULL);
    // printf("🔥 MY-OWN-MALLOC BTW (%zu)\n", size);
    // fflush(stdout);
    write(1, "🔥 MY MALLOC CALLED\n", 20);
    return NULL;
    if (size == 0 || size > MAX_ALLOC){
        return NULL;
    }
    printf("🔥 MY-OWN-MALLOC BTW (%zu)\n", size);
    // at 1st malloc only 
    if (!g_malloc.tiny && size <= TINY_MAX){ // [TINY]
        init_block_zone(1);
    }
    if (!g_malloc.small && size > TINY_MAX && size <= SMALL_MAX){ // [SMALL]
        init_block_zone(2);
    }

    
    //  bloc libre selon la taille
    t_block *block = NULL;
    if (size <= TINY_MAX) // tiny
    {
        block = find_free_block(BLOCK_TINY, size);
    } 
    else if (size <= SMALL_MAX) // small
    {
        block = find_free_block(BLOCK_SMALL, size);
    } 
    else { // large
        block = find_free_block(BLOCK_LARGE, size); // rare, pour réutiliser large déjà alloc
    }


    // bloc libre existe → réutiliser
    if (block) {
        block->free = 0; 
    } 
    // LARGE block
    else if (size > SMALL_MAX) // create a new block (pour LARGE only)
    { 
        block = allocate_block(size);
        if (!block)
            return NULL;
        block->type = BLOCK_LARGE;
        block->next = NULL;

        // push
        if (!g_malloc.head)
            g_malloc.head = (block);
        else
        {
            t_block *c = g_malloc.head;
            while (c->next)
                c = c->next;
            c->next = block;
        }
    }
    // SMALL / MEDIUM block aucuns libre found(zone pleine)
    else {
        return NULL; 
    }
    // return usable space + 1 after *ptr
    return (void *)(block + 1);
}


