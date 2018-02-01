//l
// Created by nils on 1/30/18.
//

#include "malloc.h"

static size_t   page_size = 0; // todo : check si la fonction est compilée dans le binaire
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
	last_chunk = to_add;
}

mbool   reduce_heap(chunk *to_free)
{
	long long int   to_release;

	printf("page_padding = %zu\n", page_padding);
	if (!page_size)
		page_size = (size_t) getpagesize();
	if (IS_EXIST(to_free->mchunk_size)) {
		return FAILURE;
	} else if (clean_size(to_free->mchunk_size) + page_padding > page_size) {
		printf("decrease: 0x%p\n", to_free);
		brk_addr -= clean_size(to_free->mchunk_size);
		to_release = (long long int) align_reduce_heap(clean_size(to_free->mchunk_size), page_padding);
		page_padding = 0;
		remove_last(to_free);
		sbrk(-to_release);
		return SUCCESS;
	} else {
		printf("not decrease: 0x%p\n", to_free);
		brk_addr -= clean_size(to_free->mchunk_size);
		remove_last(to_free);
		return SUCCESS;
	}
	printf("page_padding = %zu\n", page_padding);
}

void    *increase_heap(size_t request)
{
	printf("page_padding = %zu\n", page_padding);
	if (!page_size)
		page_size = (size_t) getpagesize();
	if (page_padding >= request) {
		page_padding -= request;
		brk_addr += request;
		printf("not increase: %zu\n", request);
	} else {
		request -= page_padding;
		page_padding = ((page_size) - (request % page_size));
		brk_addr = sbrk(align_increase_heap(request, page_size)) + request;
		printf("increase: %zu\n", request);
	}
	add_last(brk_addr - request);
	printf("page_padding = %zu\n", page_padding);
	return brk_addr - request;
}

void    show_alloc_mem() {
	printf("----HEAP_START----\n");
	printf("break: 0x%p\n", brk_addr);
	dump_memory(last_chunk);
	printf("----HEAP_END----\n");
}