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
	/*static int      nb_call = 0;*/
	void    *ptr;

	/*printf("%s, %s, call number %d\n", __FILE__, __func__, nb_call++);*/
	trylock_thread();
	show_alloc_mem();
	ptr = allocate(size, &free_tree);
	show_alloc_mem();
	unlock_thread();
	return ptr;
}

void    *realloc(void *mem, size_t size)
{
	/*static int      nb_call = 0;*/
	void    *ptr;

	/*printf("%s, %s, call number %d\n", __FILE__, __func__, nb_call++);*/
	if (mem) {
		trylock_thread();
		show_alloc_mem();
		discharge(mem, &free_tree);
		ptr = allocate(size, &free_tree);
		show_alloc_mem();
		unlock_thread();
		return ptr;
	}
	return NULL;
}

void    *calloc(size_t nmemb, size_t size)
{
	size_t  request = nmemb * size;
	/*static int      nb_call = 0;*/
	void    *ptr;

	/*printf("%s, %s, call number %d\n", __FILE__, __func__, nb_call++);*/
	if (request == 0)
		return NULL;
	trylock_thread();
	show_alloc_mem();
	ptr = allocate(request, &free_tree);
	clear_mem(ptr);
	show_alloc_mem();
	unlock_thread();
	return ptr;
}

void	free(void *mem)
{
	/*static int      nb_call = 0;*/

	/*printf("%s, %s, call number %d\n", __FILE__, __func__, nb_call++);*/
	if (mem) {
		trylock_thread();
		show_alloc_mem();
		discharge(mem, &free_tree);
		show_alloc_mem();
		unlock_thread();
	}
}
