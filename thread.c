/*
** EPITECH PROJECT, 2018
** malloc
** File description:
** a malloc implementation in c
*/

#include "malloc.h"

static pthread_mutex_t		mutex = PTHREAD_MUTEX_INITIALIZER;

void			lock_thread()
{
	if (pthread_mutex_trylock(&mutex) != 0)
		exit(0);
}

void			unlock_thread()
{
	if (pthread_mutex_unlock(&mutex) != 0)
		exit(0);
}
