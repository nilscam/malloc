//
// Created by nils on 2/1/18.
//

#include "malloc.h"

void    dump_memory(chunk *last_chunk) {
	if (last_chunk) {
		if (IS_EXIST(last_chunk->mchunk_prev_size))
			dump_memory(PREV(last_chunk));
		printf("0x%p - 0x%p: %zu bytes\n", last_chunk,
		       NEXT(last_chunk), clean_size(last_chunk->mchunk_size));
		print_header(last_chunk);
		printf("\n");
		if (last_chunk->mchunk_size > 100000000 || last_chunk->mchunk_prev_size > 10000000000)
			exit(84);
	}

}