/*
* Created by nils on 1/27/18.
*/

#include "malloc.h"

void    clear_mem(void *ptr)
{
	chunk   *chk = mem2chunk(ptr);

	memset(ptr, 0, schunk2smem(clean_size(chk->mchunk_size)));
}

void    *create_new_chunck(size_t request)
{
	chunk   *new = increase_heap(request);

	new->mchunk_size = request;
	UNSET_FREE(new->mchunk_size);
	UNSET_USED(new->mchunk_size);
	UNSET_EXIST(new->mchunk_size);
	return chunk2mem(new);
}

void    split_chunk(chunk **free_tree, chunk *to_split, size_t request)
{
	size_t  full_size;
	chunk   *second;
	chunk   *next;

	if (to_split->mchunk_size >= (request + MIN_CHUNK_SIZE)) {
		full_size = clean_size(to_split->mchunk_size);
		to_split->mchunk_size -= (full_size - request);
		second = NEXT(to_split);
		second->mchunk_prev_size = to_split->mchunk_size;
		second->mchunk_size = (full_size - request);
		SET_EXIST(to_split->mchunk_size);
		UNSET_FREE(to_split->mchunk_size);
		SET_FREE(second->mchunk_size);
		if (IS_EXIST(to_split->mchunk_size)) {
			SET_EXIST(second->mchunk_size);
			next = NEXT(second);
			next->mchunk_prev_size = second->mchunk_size;
		}
		add_to_tree(second, free_tree);
	} else {
		UNSET_FREE(to_split->mchunk_size);
		if (IS_EXIST(to_split->mchunk_size)) {
			next = NEXT(to_split);
			next->mchunk_prev_size = to_split->mchunk_size;
		}
	}
}

void    *allocate(size_t request, chunk **free_tree) {
	chunk *best_spot;


	request = request2chunk(request);
	if (request_oor(request)) {
		errno = ENOMEM;
		return NULL;
	} else if ((best_spot = search_best_spot(request, *free_tree))) {
		*free_tree = remove_from_tree(*free_tree, best_spot);
		split_chunk(free_tree, best_spot, request);
		return chunk2mem(best_spot);
	}
	return create_new_chunck(request);
}