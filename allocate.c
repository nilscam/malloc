//
// Created by nils on 1/27/18.
//

#include "malloc.h"

static void     *last_chunck = NULL;
static void     *first_free_chunck = NULL;

void    *cut_chunk(chunck *chunk, size_t request, clink save)
{
	//lif (request <= )
}

void    *recycle(size_t request)
{
	chunck  *it = first_free_chunck;
	clink   tmp = 0;
	clink   save = 0;

	while (it) {
		if (it->mchunk_size >= request) {
			//UNSET_FREE(&it->mchunk_size);
			return cut_chunk(it, request, save);
			//return (void*)it;
		}
		tmp = ((clink)it);
		it = (chunck*)(save ^ it->xor_link);
		save = tmp;
	}
	return it;
}

void    *create_new_chunck(size_t request)
{
	void    *new = sbrk(0);

	brk(sbrk(request));
	if (last_chunck)
		SET_PREV_CHUNK_SIZE(new, last_chunck); // bitmask pas géré
	else
		((chunck*)new)->mchunk_prev_size = NULL;
	((chunck*)new)->mchunk_prev_size = request;
	last_chunck = new;
	return chunk2mem(new);
}

void    *allocate(void *heap_start, size_t request)
{
	void    *new;

	request = request2chunck(request);
	if (request_oor(request)) {
		errno = ENOMEM;
		return NULL;
	}
	if (free_chunck)
		if (new = recycle(request))
			return new;
	return create_new_chunck(request);
}