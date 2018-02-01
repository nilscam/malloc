//
// Created by nils on 1/27/18.
//

#include "malloc.h"

void    *create_new_chunck(size_t request)
{
	chunk   *new = increase_heap(request);

	new->mchunk_size = request;
	UNSET_FREE(new->mchunk_size);
	UNSET_USED(new->mchunk_size);
	UNSET_EXIST(new->mchunk_size);
	return chunk2mem(new);
}

chunk   *search_best_spot(size_t request, chunk *root_tree)
{
	chunk   *it = root_tree;
	chunk   *best = NULL;

	while (it && it->mchunk_size != request) {
		if (clean_size(it->mchunk_size) >= request &&
			(!best || clean_size(best->mchunk_size) >
			          clean_size(it->mchunk_size))) {
			best = it;
			//goto start_tricks;
			if (clean_size(best->mchunk_size) == request)
				return best;
		}
		//end_tricks:
		it = move_tree(it, request);
	}
	return best;
	/*start_tricks:
	if (clean_size(best->mchunk_size) == request)
		return best;
	goto end_tricks;*/
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
		second->mchunk_prev_size = to_split->mchunk_size:
		second->mchunk_size = (full_size - request);
		SET_EXIST(to_split->mchunk_size);
		UNSET_FREE(to_split->mchunk_size);
		SET_FREE(second->mchunk_size);
		if (IS_EXIST(to_split->mchunk_size)) {
			SET_EXIST(second->mchunk_size);
			next = NEXT(second);
			next->mchunk_prev_size = second->mchunk_size;
		}
		add_to_tree(to_split, free_tree);
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
		return best_spot;
	}
	return create_new_chunck(request);
}