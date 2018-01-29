//
// Created by nils on 1/27/18.
//

#include "malloc.h"

static chunk    *last_chunk = NULL;
//static chunk  *first_free_chunck = NULL;

void    *create_new_chunck(size_t request)
{
	chunk   *new = sbrk(0);

	brk(sbrk(request));
	if (last_chunk)
		SET_PREV_CHUNK_SIZE(new, last_chunk); // bitmask pas géré
	else
		new->mchunk_prev_size = NULL;
	new->mchunk_prev_size = request;
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

