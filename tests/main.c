// main.c
#include <stdio.h>
#include "malloc.h"

int main() {
    printf("==================  MALLOC TESTS ==================\n");

    // --- TINY ---
    char *t1 = (char *)malloc(12);
    void *t2 = malloc(24);
    void *t3 = malloc(62);
    void *t4 = malloc(1);

    // --- SMALL ---
    char *s1 = (char *)malloc(256);
    void *s2 = malloc(304);

    // --- LARGE ---
    void *l1 = malloc(5000);

    (void)t1;
    (void)t2;
    (void)t3;
    (void)s1;
    (void)s2;
    (void)l1;
    t1[0] = 'a';
    t1[1] = 'b';
    t1[2] = 'c';
    t1[3] = '\n';
    printf("t1 = %s", t1);
    strcpy(s1, "ABCDEGLKFEIJERIJR&ÉÀ1231235030430404040340340\n");
    printf("s1 = %s", s1);
    show_alloc_mem();

    printf("==================  AFTER FREE ==================\n");
    free(t1);
    free(t3);
    free(l1);
    
    show_alloc_mem();

	malloc(1024 * 1024);
	malloc(1024 * 1024 * 16);
	malloc(1024 * 1024 * 128);
    show_alloc_mem();

    // stress test
    /*
    int i; 
	void *addr; 
	i = 0; 
	while (i < 256) // large
	{ 
		addr = malloc(2501); 
		((char *)addr)[0] = 42; 
		i++; 
	} 
    i = 0; 
	while (i < 256) // small
	{ 
		addr = (char * )malloc(1024); 
		// addr[0] = 42; 
		i++; 
	}
    i = 0; 
	while (i < 256) // tiny
	{ 
		addr = (char * )malloc(128); 
		// addr[0] = 42; 
		i++; 
	}  
    show_alloc_mem();
    */
    return 0;
}
