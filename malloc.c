/*
** EPITECH PROJECT, 2018
** 
** File description:
** 
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define FULL 0xF
#define MAX_OPTI_SIZE 0x3FFFFFFFFFFFFFFF

void	*heap_start;
void    *heap_end;


size_t	my_opalloc(size_t n)
{
        size_t	nb = n - 1;
	size_t	drop = 31;

	if (n > MAX_OPTI_SIZE)
		return n;
	while (drop > 0)
		nb |= nb >> drop--;
	return (nb + 1) << 1;
}

void	*malloc(size_t size)
{
	printf("nb = %zu\n", my_opalloc(size));

	heap_start = sbrk(0);

	printf("brk = %d\n", brk(sbrk(100)));

	//printf("brk = l%d\n", brk(heap_start));
	
	return heap_start;
}

void	free(__attribute__((unused))void *data)
{
	brk(heap_start);
}
