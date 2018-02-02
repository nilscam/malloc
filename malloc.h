//
// Created by nils on 1/26/18.
//

#ifndef _malloc_H_
#define _malloc_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <zconf.h>
#include <unistd.h>
#include <pthread.h>

/*
 * Representation of a chunk
 *
 * +---------------------------+
 * |    Size of prev Chunk    |
 * +---------------------------+
 * |       Size of chunk      |
 * +---------------------------+
 * | row data : min size 8     |
 * | // if free                |
 * | // xor_link: a xor linked |
 * | // list implementation    |printf
 * +---------------------------+
 *
 * Size of chunk representation (4 bytes):
 * 11111111111111111111111111111[A][B][C]
 * 3 lowers bits are used to stock information about the chunk
 *
 * A: 1 if the chunk is used by a thread
 *    0 otherwise
 *
 * B: 1 if the chunk is used
 *    0 if the chunk is free
 *
 * C: 1 if there is a next/prev chunk
 *    0 otherwise
 *
 * if B is set to 1, the row data contain a xor_link
 */

struct  s_chunk {
	size_t  mchunk_prev_size;  /* Size of previous chunk (if free).  */ // set by SET_PREV_CHUNK_SIZE Macro
	size_t	mchunk_size;       /* Size in bytes, including overhead. */ // set by SET_CHUNK_SIZE Macro

	// only if free
	struct s_chunk  *smaller;
	struct s_chunk  *bigger;
};

typedef struct s_chunk chunk;

/* Malloc boolean */
typedef char mbool;
#define SUCCESS 0
#define FAILURE 1
/* */


/* mask */
#define CHUNK_ALIGN_MASK 0x7
#define REVERSE_CHUNK_ALIGN_MASK ~0x7
#define USED_MASK 4
#define FREE_MASK 2
#define EXIST_MASK 1
/* */

/* SIZE brute */
#define CHUNK_HEADER_SIZE (offsetof(struct s_chunk, smaller))
#define MIN_SIZE 16
#define MIN_CHUNK_SIZE (MIN_SIZE + CHUNK_HEADER_SIZE)
/* */

/* management chunk */
#define NEXT(chk) ((chunk*)(clean_size((chk)->mchunk_size) + \
		((unsigned long long int)(chk))))
#define PREV(chk) ((chunk*)(((unsigned long long int)(chk)) - \
		clean_size((chk)->mchunk_prev_size)))
#define move_tree(it, size_to_add) ((clean_size((it)->mchunk_size) > clean_size((size_to_add))) ? \
					(it)->smaller : (it)->bigger)
/* */

/* chunk comparaisons */
#define cmp_chunk_grt(chk1, chk2) (clean_size((chk1)->mchunk_size) > \
					clean_size((chk2)->mchunk_size))
#define cmp_chunk_lrt(chk1, chk2) (clean_size((chk1)->mchunk_size) < \
					clean_size((chk2)->mchunk_size))
#define cmp_chunk_lrt_oe(chk1, chk2) (clean_size((chk1)->mchunk_size) <= \
					clean_size((chk2)->mchunk_size))
/* */

/* ptr/size conversion */
#define chunk2mem(p)   ((void*)((char*)(p) + CHUNK_HEADER_SIZE))
#define mem2chunk(mem) ((chunk*)(((char*)(mem)) - CHUNK_HEADER_SIZE))
#define schunk2smem(s) ((s) - CHUNK_HEADER_SIZE)
#define smem2schunk(s) ((s) + CHUCK_HEADER_SIZE)
#define clean_size(size) ((size) & REVERSE_CHUNK_ALIGN_MASK)
/* */


/* logic calcul */
#define request2mem(size) (clean_size((size) + CHUNK_ALIGN_MASK) + CHUNK_HEADER_SIZE)
#define request2chunk(size) (((request2mem(size)) > MIN_CHUNK_SIZE) ? request2mem(size) : MIN_CHUNK_SIZE)
#define request_oor(size) ((size) >= (-2*CHUNK_HEADER_SIZE))
#define combine_size_chunk(first, second) ((clean_size((first)->mchunk_size))\
				+ (clean_size((second)->mchunk_size))\
				+ ((second)->mchunk_size & CHUNK_ALIGN_MASK))
#define align_increase_heap(size, pagesize) (((size) + (pagesize) - 1) & ~((pagesize) - 1))
#define align_reduce_heap(size, pagesize) ((size) & ~((pagesize) -1))
/* */

/* setters / conditions */
#define IS_EXIST(size_ptr) ((size_ptr) & EXIST_MASK)
#define SET_EXIST(size_ptr) ((size_ptr) |= EXIST_MASK)
#define UNSET_EXIST(size_ptr) ((size_ptr) &= ~EXIST_MASK)

#define IS_FREE(size_ptr) ((((size_ptr) & FREE_MASK) >> 1) ^ 1)
#define SET_FREE(size_ptr) ((size_ptr) &= ~FREE_MASK)
#define UNSET_FREE(size_ptr) ((size_ptr) |= FREE_MASK)

#define IS_USED(size_ptr) (((size_ptr) & USED_MASK) >> 2)
#define SET_USED(size_ptr) ((size_ptr) |= USED_MASK)
#define UNSET_USED(size_ptr) ((size_ptr) &= ~USED_MASK)
/* */

/* debug */
#define print_header(chk) printf("+----------------------------------+\n\
|%34zu|\n\
|%34zu|\n\
+----------------------------------+\n",\
(chk)->mchunk_prev_size, (chk)->mchunk_size)

/* */

void    *allocate(size_t, chunk **free_tree);
void    discharge(void *, chunk **);
mbool   reduce_heap(chunk *);
void    *increase_heap(size_t);
chunk   *remove_from_tree(chunk *, chunk *);
void    add_to_tree(chunk *, chunk **);
void    show_alloc_mem();
void    dump_memory(chunk *);

#endif //_malloc_H_
