//
// Created by nils on 1/26/18.
//

#ifndef _malloc_H_
#define _malloc_H_

#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <zconf.h>

/*
 * Representation of a chunck
 *
 * +---------------------------+
 * |    Size of prev Chunck    |
 * +---------------------------+
 * |       Size of chunck      |
 * +---------------------------+
 * | row data : min size 8     |
 * | // if free                |
 * | // xor_link: a xor linked |
 * | // list implementation    |
 * +---------------------------+
 *
 * Size of chunck representation (4 bytes):
 * 11111111111111111111111111111[A][B][C]
 * 3 lowers bits are used to stock information about the chunck
 *
 * A: 1 if the chunck is used by a thread
 *    0 otherwise
 *
 * B: 1 if the chunck is used
 *    0 if the chunck is free
 *
 * C: 1 if there is a next/prev chunck
 *    0 otherwise
 *
 * if B is set to 1, the row data contain a xor_link
 */

typedef unsigned long int       clink;
struct  s_chunck {
	size_t  mchunk_prev_size;  /* Size of previous chunk (if free).  */ // set by SET_PREV_CHUNK_SIZE Macro
	size_t	mchunk_size;       /* Size in bytes, including overhead. */ // set by SET_CHUNK_SIZE Macro

	clink   xor_link; // only for free node
};
typedef struct s_chunck chunck;

#define CHUNCK_ALIGN_MASK 0x7
#define REVERSE_CUNCK_ALIGN_MASK ~0x7

#define USED_MASK 4
#define FREE_MASK 2
#define EXIST_MASK 1

#define NEXT(prev, actual) (chunck*)((prev) ^ (actual))
#define PREV(next, actual) (chunck*)((next) ^ (actual))

#define CHUNCK_HEADER_SIZE (offsetof(struct s_chunck, xor_link)) // header size
#define MIN_SIZE 8
#define MIN_CHUNCK_SIZE (MIN_SIZE + CHUNCK_HEADER_SIZE)



#define chunk2mem(p)   ((void*)((char*)(p) + CHUNCK_HEADER_SIZE)) // convert a chunck ptr to a ptr on it's mem
#define mem2chunk(mem) ((chunck*)(((char*)(mem)) - CHUNCK_HEADER_SIZE)) // convert a mem ptr to a ptr on it's chunck

#define schunck2smem(s) ((s) - CHUNCK_HEADER_SIZE)
#define smem2schunck(s) ((s) + CHUNCK_HEADER_SIZE)


#define request2mem(size) (((size) + CHUNCK_ALIGN_MASK) & REVERSE_CUNCK_ALIGN_MASK)
#define request2chunck(size) (((request2mem(size)) > MIN_SIZE) ? request2mem(size) + CHUNCK_HEADER_SIZE : MIN_CHUNCK_SIZE)
#define request_oor(size) ((size) >= (-2*CHUNCK_HEADER_SIZE))

#define IS_EXIST(size_ptr) ((*(size_ptr)) & EXIST_MASK)
#define SET_EXIST(size_ptr) ((*(size_ptr)) |= EXIST_MASK)
#define UNSET_EXIST(size_ptr) ((*(size_ptr)) &= ~EXIST_MASK)

#define IS_FREE(size_ptr) (((*(size_ptr)) & FREE_MASK) >> 1)
#define SET_FREE(size_ptr) ((*(size_ptr)) &= ~FREE_MASK)
#define UNSET_FREE(size_ptr) ((*(size_ptr)) |= FREE_MASK)

#define IS_USED(size_ptr) (((*(size_ptr)) & USED_MASK) >> 2)
#define SET_USED(size_ptr) ((*(size_ptr)) |= USED_MASK)
#define UNSET_USED(size_ptr) ((*(size_ptr)) &= ~USED_MASK)



#define SET_PREV_CHUNK_SIZE(cur, prev) (((chunck*)(cur))->mchunk_prev_size = \
	((cur) - (prev)) + ((chunck*)(prev))->mchunk_size & CHUNCK_ALIGN_MASK)

void    *allocate(void *, size_t);

#endif //_malloc_H_
