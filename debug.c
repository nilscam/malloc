/*
* Created by nils on 2/1/18.
*/

#include "malloc.h"


void    dump_memory(chunk *last_chunk) {
	if (last_chunk) {
		if (IS_EXIST(last_chunk->mchunk_prev_size))
			dump_memory(PREV(last_chunk));
		printf("0x%p - 0x%p: %lu bytes\n", ((void*)last_chunk),
		       ((void*)NEXT(last_chunk)), clean_size(last_chunk->mchunk_size));
		print_header(last_chunk);
		printf("\n");
	}

}
