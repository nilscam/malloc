//
// Created by nils on 1/26/18.
//

#ifndef _malloc_H_
#define _malloc_H_

#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <zconf.h>
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
 * | // list implementation    |
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
#define NEXT(chunk) ((chunk)->mchunk_size + (chunk))
#define PREV(chunk) ((chunk) - (chunk)->mchunk_prev_size)
#define move_tree(it, size_to_add) (((it)->mchunk_size > (size_to_add)) ? \
					(it)->smaller : (it)->bigger)
/* */

/* ptr/size conversion */
#define chunk2mem(p)   ((void*)((char*)(p) + CHUNK_HEADER_SIZE))
#define mem2chunk(mem) ((chunk*)(((char*)(mem)) - CHUNK_HEADER_SIZE))
#define schunk2smem(s) ((s) - CHUNK_HEADER_SIZE)
#define smem2schunk(s) ((s) + CHUCK_HEADER_SIZE)
/* */


/* logic calcul */
#define request2mem(size) (((size) + CHUNK_ALIGN_MASK) & REVERSE_CHUNK_ALIGN_MASK)
#define request2chunk(size) (((request2mem(size)) > MIN_SIZE) ? request2mem(size) + CHUNK_HEADER_SIZE : MIN_CHUNK_SIZE)
#define request_oor(size) ((size) >= (-2*CHUNK_HEADER_SIZE))
#define combine_size_chunk(first, second) (((first)->mchunk_size & REVERSE_CHUNK_ALIGN_MASK) \
					+ ((second)->mchunk_size & REVERSE_CHUNK_ALIGN_MASK) \
					+ ((second)->mchunk_size & CHUNK_ALIGN_MASK))
/* */

/* setters / conditions */
#define IS_EXIST(size_ptr) ((size_ptr) & EXIST_MASK)
#define SET_EXIST(size_ptr) ((size_ptr) |= EXIST_MASK)
#define UNSET_EXIST(size_ptr) ((size_ptr) &= ~EXIST_MASK)

#define IS_FREE(size_ptr) (((size_ptr) & FREE_MASK) >> 1)
#define SET_FREE(size_ptr) ((size_ptr) &= ~FREE_MASK)
#define UNSET_FREE(size_ptr) ((size_ptr) |= FREE_MASK)

#define IS_USED(size_ptr) (((size_ptr) & USED_MASK) >> 2)
#define SET_USED(size_ptr) ((size_ptr) |= USED_MASK)
#define UNSET_USED(size_ptr) ((size_ptr) &= ~USED_MASK)
/* */


void    *allocate(size_t);
void    discharge(void *, chunk **);

#endif //_malloc_H_
