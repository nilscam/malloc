/*
** EPITECH PROJECT, 2018
** malloc
** File description:
** a malloc implementation in c
*/

#include "malloc.h"

static chunk	*free_tree = NULL;

void	*malloc(size_t size)
{
	void	*ptr = NULL;

	dump_func;
	show_alloc_mem();
	if (size) {
		lock_thread();
		ptr = allocate(size, &free_tree);
		unlock_thread();
	}
	show_alloc_mem();
	padd_debug;
	return ptr;
}

void	*realloc(void *mem, size_t size)
{
	void	*ptr;

	dump_func;
	show_alloc_mem();
	lock_thread();
	if (mem)
		discharge(mem, &free_tree);
	if (size) {
		ptr = allocate(size, &free_tree);
		unlock_thread();
		show_alloc_mem();
		padd_debug;
		return ptr;
	}
	unlock_thread();
	show_alloc_mem();
	padd_debug;
	return NULL;
}

void	*calloc(size_t nmemb, size_t size)
{
	size_t	request = nmemb * size;
	void	*ptr;

	dump_func;
	if (request == 0)
		return NULL;
	show_alloc_mem();
	lock_thread();
	ptr = allocate(request, &free_tree);
	clear_mem(ptr);
	unlock_thread();
	show_alloc_mem();
	padd_debug;
	return ptr;
}

void	free(void *mem)
{
	dump_func;
	if (mem) {
		show_alloc_mem();
		lock_thread();
		discharge(mem, &free_tree);
		unlock_thread();
		show_alloc_mem();
	}
	padd_debug;
}
