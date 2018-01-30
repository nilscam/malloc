/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#include "malloc.h"

static chunk  *free_tree = NULL;

//brk place le break à une adresse
//sbrk return la somme de l'adresse de break et du parametre

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
	ptr = allocate(size);
	unlock_thread();
	return ptr;
}

void    *realloc(__attribute__((unused))void *ptr, __attribute__((unused))size_t size)
{
	return NULL;
}

void	free(void *mem)
{
	trylock_thread();
	discharge(mem, &free_tree);
	unlock_thread();
}
