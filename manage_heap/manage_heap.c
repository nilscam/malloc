/*
** EPITECH PROJECT, 2018
** malloc
** File description:
** a malloc implementation in c
*/

#include "manage_heap.h"

void	remove_last(chunk *to_remove)
{
	chunk	**last_chunk = get_last_chunk();

	if (IS_EXIST(to_remove->mchunk_prev_size)) {
		*last_chunk = PREV(to_remove);
		UNSET_EXIST((*last_chunk)->mchunk_size);
	} else {
		*last_chunk = NULL;
	}
}

void	add_last(chunk *to_add)
{
	chunk	**last_chunk = get_last_chunk();

	if (*last_chunk) {
		to_add->mchunk_prev_size = (*last_chunk)->mchunk_size;
		SET_EXIST((*last_chunk)->mchunk_size);
		SET_EXIST(to_add->mchunk_prev_size);
	} else {
		to_add->mchunk_prev_size = 0;
	}
	*last_chunk = to_add;
}

mbool	reduce_heap(chunk *to_free)
{
	long int	to_release;
	size_t		*page_padding = my_get_page_padding();
	void		**brk_addr = get_brk_addr();

	if (IS_EXIST(to_free->mchunk_size)) {
		return FAILURE;
	} else if (IS_REDUCIBLE(to_free, *page_padding, my_page_size())) {
		*brk_addr = REDUCE_BRK(*brk_addr, to_free);
		to_release = GET_HOW_MUCH_TO_RELEASE(to_free, my_page_size());
		*page_padding = (*page_padding
				+ clean_size(to_free->mchunk_size))
				% my_page_size();
		remove_last(to_free);
		sbrk(-to_release);
	} else {
		*brk_addr = REDUCE_BRK(*brk_addr, to_free);
		*page_padding += clean_size(to_free->mchunk_size);
		remove_last(to_free);
	}
		return SUCCESS;
}

void	*increase_heap(size_t request)
{
	size_t	adjusted_request;
	size_t	*page_padding = my_get_page_padding();
	void	**brk_addr = get_brk_addr();

	if (*page_padding >= request) {
		*page_padding -= request;
	} else {
		adjusted_request = request - *page_padding;
		*page_padding = ((my_page_size()) -
				(adjusted_request % my_page_size()));
		sbrk(align_increase_heap(adjusted_request, my_page_size()));
	}
	*brk_addr = ((char*)(*brk_addr)) + request;
	add_last((chunk*)(((char*)(*brk_addr)) - request));
	return ((char*)(*brk_addr)) - request;
}

void	show_alloc_mem()
{
#ifdef M_DEBUG
	write(1, "----HEAP_START----\nbreak: ", 26);
	put_addr(*get_brk_addr());
	write(1, "\n", 1);
	dump_memory(*get_last_chunk());
	write(1, "----HEAP_END----\n", 17);
#endif
}
