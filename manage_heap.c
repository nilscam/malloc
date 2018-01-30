//
// Created by nils on 1/30/18.
//

#include "malloc.h"

static size_t   page_size = _SC_PAGESIZE; // todo : check si la fonction est compilée dans le binaire
static size_t   page_padding = 0;
static void     *brk_addr = NULL;

mbool   reduce_heap(chunk *to_free)
{
	if (brk_addr - to_free->mchunk_size)
}

mbool   increase_heap()
{

}