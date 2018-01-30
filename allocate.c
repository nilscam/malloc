//
// Created by nils on 1/27/18.
//

#include "malloc.h"

static chunk    *last_chunk = NULL;

void    *create_new_chunck(size_t request)
{
	chunk   *new = sbrk(0);

	brk(sbrk(request));
	if (last_chunk) {
		new->mchunk_prev_size = last_chunk->mchunk_size;
		SET_EXIST(last_chunk->mchunk_size);
		SET_EXIST(new->mchunk_prev_size);
	} else {
		new->mchunk_prev_size = 0;
	}
	new->mchunk_size = request;
	UNSET_FREE(new->mchunk_size);
	UNSET_USED(new->mchunk_size);
	UNSET_EXIST(new->mchunk_size);
	last_chunk = new;
	return chunk2mem(new);
}

void    *allocate(size_t request)
{
	request = request2chunk(request);
	if (request_oor(request)) {
		errno = ENOMEM;
		return NULL;
	}
	return create_new_chunck(request);
}