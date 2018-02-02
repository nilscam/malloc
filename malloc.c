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
	void    *ptr;

	trylock_thread();
	printf("%s, %s, %zu\n", __FILE__, __func__, size);
	show_alloc_mem();
	ptr = allocate(size, &free_tree);
	print_header(mem2chunk((chunk*)ptr));
	show_alloc_mem();
	printf("\n");
	unlock_thread();
	return ptr;
}

void    *realloc(void *mem, size_t size) // todo : se fait tout seul
{
	void    *ptr;

	if (mem) {
		trylock_thread();
		printf("%s, %s, %zu, %p\n", __FILE__, __func__, size, mem);
		show_alloc_mem();
		discharge(mem, &free_tree);
		ptr = allocate(size, &free_tree);
		show_alloc_mem();
		printf("\n");
		unlock_thread();
		return ptr;
	}
	return NULL;
}

void	free(void *mem)
{
	if (mem) {
		trylock_thread();
		printf("\n%s, %s, %p\n", __FILE__, __func__, mem);
		show_alloc_mem();
		discharge(mem, &free_tree);
		show_alloc_mem();
		printf("\n");
		unlock_thread();
	}
}
