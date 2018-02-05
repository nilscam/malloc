/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#include "malloc.h"

static chunk  *free_tree = NULL;

pthread_mutex_t		mutex = PTHREAD_MUTEX_INITIALIZER;

void			*trylock_thread()
{
	if (pthread_mutex_trylock(&mutex) != 0)
		exit(0);
	return (NULL);
}

void			*unlock_thread()
{
	if (pthread_mutex_unlock(&mutex) != 0)
		exit(0);
	return (NULL);
}

void	*malloc(size_t size)
{
	void    *ptr = NULL;

	dump_func;
	show_alloc_mem();
	if (size) {
		trylock_thread();
		ptr = allocate(size, &free_tree);
		unlock_thread();
	}
	show_alloc_mem();
	padd_debug;
	return ptr;
}

void    *realloc(void *mem, size_t size)
{
	void    *ptr;

	dump_func;
	show_alloc_mem();
	trylock_thread();
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

void    *calloc(size_t nmemb, size_t size)
{
	size_t  request = nmemb * size;
	void    *ptr;

	dump_func;
	if (request == 0)
		return NULL;
	show_alloc_mem();
	trylock_thread();
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
		trylock_thread();
		discharge(mem, &free_tree);
		unlock_thread();
		show_alloc_mem();
	}
	padd_debug;
}
