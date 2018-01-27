/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#include "malloc.h"

//brk place le break à une adresse
//sbrk return la somme de l'adresse de break et du parametre

static void     *heap_start = NULL;

void	*malloc(size_t size)
{
	if (!heap_start)
		heap_start = sbrk(0);
	return allocate(heap_start, size);
}

void    *realloc(__attribute__((unused))void *ptr, __attribute__((unused))size_t size)
{
	return NULL;
}

void	free(__attribute__((unused))void *data)
{
}
