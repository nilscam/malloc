/*
** EPITECH PROJECT, 2018
** malloc
** File description:
** a malloc implementation in c
*/

#include "manage_heap.h"

static size_t	page_size = 0;
static size_t	page_padding = 0;
static void	*brk_addr = NULL;
static chunk	*last_chunk = NULL;

void	remove_last(chunk *to_remove)
{
	if (IS_EXIST(to_remove->mchunk_prev_size)) {
		last_chunk = PREV(to_remove);
		UNSET_EXIST(last_chunk->mchunk_size);
	} else {
		last_chunk = NULL;
	}
}

void	add_last(chunk *to_add)
{
	if (last_chunk) {
		to_add->mchunk_prev_size = last_chunk->mchunk_size;
		SET_EXIST(last_chunk->mchunk_size);
		SET_EXIST(to_add->mchunk_prev_size);
	} else {
		to_add->mchunk_prev_size = 0;
	}
	last_chunk = to_add;
}

mbool	reduce_heap(chunk *to_free)
{
	long int	to_release;

	if (!page_size)
		page_size = ((size_t)getpagesize());
	if (IS_EXIST(to_free->mchunk_size)) {
		return FAILURE;
	} else if (IS_REDUCIBLE(to_free, page_padding, page_size)) {
		brk_addr = REDUCE_BRK(brk_addr, to_free);
		to_release = GET_HOW_MUCH_TO_RELEASE(to_free, page_size);
		page_padding = (page_padding +
				clean_size(to_free->mchunk_size)) % page_size;
		remove_last(to_free);
		sbrk(-to_release);
		return SUCCESS;
	} else {
		brk_addr = REDUCE_BRK(brk_addr, to_free);
		page_padding += clean_size(to_free->mchunk_size);
		remove_last(to_free);
		return SUCCESS;
	}
}

void	*increase_heap(size_t request)
{
	size_t	adjusted_request;

	if (!page_size || !brk_addr) {
		page_size = ((size_t)getpagesize());
		brk_addr = sbrk(0);
	}
	if (page_padding >= request) {
		page_padding -= request;
	} else {
		adjusted_request = request - page_padding;
		page_padding = ((page_size) - (adjusted_request % page_size));
		sbrk(align_increase_heap(adjusted_request, page_size));
	}
	brk_addr = ((char*)brk_addr) + request;
	add_last((chunk*)(((char*)brk_addr) - request));
	return ((char*)brk_addr) - request;
}

void	show_alloc_mem()
{
#ifdef M_DEBUG
	write(1, "----HEAP_START----\nbreak: ", 26);
	put_addr(brk_addr);
	write(1, "\n", 1);
	dump_memory(last_chunk);
	write(1, "----HEAP_END----\n", 17);
#endif
}
