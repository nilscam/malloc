/*
** EPITECH PROJECT, 2018
** malloc
** File description:
** a malloc implementation in c
*/

#ifndef _manage_heap_H_
#define _manage_heap_H_

#include "../malloc.h"

/* alignement */
#define align_increase_heap(size, pagesize) (((size) + (pagesize) - 1) & \
						~((pagesize) - 1))
#define align_reduce_heap(size, pagesize) ((size) & ~((pagesize) -1))
/* */

/* calcul heap management */
#define GET_HOW_MUCH_TO_RELEASE(to_free, page_size) ((long int) \
		align_reduce_heap(clean_size(to_free->mchunk_size), page_size))
#define REDUCE_BRK(brk_addr, to_free) \
		(((char *)brk_addr) - clean_size(to_free->mchunk_size))
#define IS_REDUCIBLE(to_free, page_padding, page_size) \
	(clean_size(to_free->mchunk_size) + page_padding > page_size)
/* */

/* debug */
#define DIGITS "0123456789"
#define print_header(chk) do {\
	write(1, "+--------------START---------------+\n", 37);\
	put_nbr(((unsigned long int)((chk)->mchunk_size)));\
	write(1, "\n", 1);\
	put_nbr(((unsigned long int)((chk)->mchunk_size)));\
	write(1, "\n", 1);\
	write(1, "+---------------END----------------+\n", 37);\
} while (0)

#ifdef M_DEBUG
#define dump_func do {\
	write(1, __func__, strlen(__func__));\
	write(1, " - ", 3);\
	write(1, __FILE__, strlen(__FILE__));\
	write(1, "\n", 1);\
} while (0)
#define padd_debug write(1, "\n\n\n", 3)
#else
#define dump_func do {} while (0)
#define padd_debug do {} while (0)
#endif
/* */

#endif /* _manage_heap_H_ */
