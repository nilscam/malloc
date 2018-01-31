//
// Created by nils on 1/30/18.
//

#include "malloc.h"

static size_t   page_size = _SC_PAGESIZE; // todo : check si la fonction est compilée dans le binaire
static size_t   page_padding = 0;

mbool   set_prev(chunk *to_remove, long long int to_release)
{
	chunk   *prev;

	page_padding += clean_size(to_remove->mchunk_size);
	if (IS_EXIST(to_remove->mchunk_prev_size)) {
		prev = PREV(to_remove);
		UNSET_EXIST(prev->mchunk_size);
	}
	sbrk(-to_release);
	return SUCCESS;
}

mbool   reduce_heap(chunk *to_free)
{
	long long int   to_release;

	if (IS_EXIST(to_free->mchunk_size)) {// check si il y a un chunk après
		return FAILURE;
	} else if (clean_size(to_free->mchunk_size) + page_padding > page_size) { // check si c'est le dernier chunk
		to_release = (long long int) align_reduce_heap(clean_size(to_free->mchunk_size), page_padding);
		to_free->mchunk_size -= (to_release - page_padding);
		page_padding = 0;
		if (clean_size(to_free->mchunk_size) < MIN_CHUNK_SIZE) {
			return set_prev(to_free, to_release);
		}
		sbrk(-to_release);
	}
	return FAILURE;
}

mbool   increase_heap(size_t request)
{
	if (page_padding > request) {
		page_padding -= request;
	} else {
		request -= page_padding;
		page_padding = request % page_size;
		sbrk(align_increase_heap(request, page_size));
	}
	return SUCCESS;
}