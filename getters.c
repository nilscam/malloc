/*
** EPITECH PROJECT, 2018
** malloc
** File description:
** a malloc implementation in c
*/

#include "malloc.h"

size_t	my_get_page_size()
{
	static size_t	page_size = 0;

	if (!page_size)
		page_size = ((size_t)getpagesize());
	return page_size;
}

size_t	*my_get_page_padding()
{
	static size_t	page_padding = 0;

	return &page_padding;
}

void	**get_brk_addr()
{
	static void	*brk_addr = NULL;

	if (!brk_addr)
		brk_addr = sbrk(0);
	return &brk_addr;
}

chunk	**get_last_chunk()
{
	static chunk	*last_chunk = NULL;

	return &last_chunk;
}
