//
// Created by nils on 1/30/18.
//

#include "malloc.h"

static size_t   page_size = _SC_PAGESIZE; // todo : check si la fonction est compilée dans le binaire
static size_t   page_padding = 0;
static void     *brk_addr = NULL;
static chunk    *last_chunk = NULL;

void    set_prev_no_next(chunk *to_remove)
{
	if (IS_EXIST(to_remove->mchunk_prev_size)) {
		last_chunk = PREV(to_remove);
		UNSET_EXIST(last_chunk->mchunk_size);
	}
}

void    set_prev_next(chunk *to_add) // todo : call this method
{
	if (last_chunk)
		SET_EXIST(last_chunk->mchunk_size);
	last_chunk = to_add;
}

mbool   reduce_heap(chunk *to_free)
{
	long long int   to_release;

	if (IS_EXIST(to_free->mchunk_size)) { // check si c'est le dernier chunk
		return FAILURE;
	} else if (clean_size(to_free->mchunk_size) + page_padding > page_size) {

		brk_addr -= clean_size(to_free->mchunk_size);
		to_release = (long long int) align_reduce_heap(clean_size(to_free->mchunk_size), page_padding);
		page_padding = 0;
		set_prev_no_next(to_free);
		sbrk(-to_release);
		return SUCCESS;
	} else {
		brk_addr -= clean_size(to_free->mchunk_size);
		set_prev_no_next(to_free);
		return SUCCESS;
	}
}

void    *increase_heap(size_t request)
{
	if (page_padding >= request) {
		page_padding -= request;
		brk_addr += request;
	} else {
		request -= page_padding;
		page_padding = request % page_size;
		brk_addr = sbrk(align_increase_heap(request, page_size)) +
		        align_increase_heap(request, page_padding) - page_padding;
	}
	set_prev_next(brk_addr);
	return brk_addr;
}