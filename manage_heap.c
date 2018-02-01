//
// Created by nils on 1/30/18.
//

#include "malloc.h"

static size_t   page_size = _SC_PAGESIZE; // todo : check si la fonction est compilée dans le binaire
static size_t   page_padding = 0;
static void     *brk_addr = NULL;
static chunk    *last_chunk = NULL;

void    remove_last(chunk *to_remove)
{
	if (IS_EXIST(to_remove->mchunk_prev_size)) {
		last_chunk = PREV(to_remove);
		UNSET_EXIST(last_chunk->mchunk_size);
	}
}

void    add_last(chunk *to_add)
{
	if (last_chunk) {
		to_add->mchunk_prev_size = last_chunk->mchunk_size;
		SET_EXIST(last_chunk->mchunk_size);
		SET_EXIST(to_add->mchunk_prev_size);
	} else {
		to_add->mchunk_prev_size = 0;
	}
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
		remove_last(to_free);
		sbrk(-to_release);
		return SUCCESS;
	} else {
		brk_addr -= clean_size(to_free->mchunk_size);
		remove_last(to_free);
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
		brk_addr = sbrk(align_increase_heap(request, page_size)) + request;
	}
	add_last(brk_addr - request);
	return brk_addr - request;
}