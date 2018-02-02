/*
* Created by nils on 2/1/18.
*/

#include "malloc.h"
#define DIGITS "0123456789"

void    put_nbr(unsigned long int nb)
{
	if (nb / 10)
		put_nbr(nb / 10);
	write(1, DIGITS + nb % 10, 1);
}

void    put_addr(void *addr)
{
	write(1, "0x", 2);
	put_nbr((unsigned long int)(addr));
}

void    dump_memory(chunk *last_chunk) {
	if (last_chunk) {
		if (IS_EXIST(last_chunk->mchunk_prev_size))
			dump_memory(PREV(last_chunk));
		put_addr((void*)last_chunk);
		write(1, " - ", 3);
		put_addr((void*)NEXT(last_chunk));
		write(1, ": ", 2);
		put_nbr(clean_size(last_chunk->mchunk_size));
		write(1, " bytes\n", 7);
		print_header(last_chunk);
	}

}
