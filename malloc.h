//
// Created by nils on 1/26/18.
//

#ifndef _malloc_H_
#define _malloc_H_

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
 * B: 1 if the chunck is free
 *    0 otherwise
 *
 * C: 1 if there is a next/prev chunck
 *    0 otherwise
 *
 * if B is set to 1, the row data contain a xor_link
 */

typedef unsigned int    size;
typedef unsigned long int       link;

#define CHUNK_ALIGN_MASK 0x7
#define REVERSE_CUNCK_ALIGN_MASK ~0x7

#define USED_MASK 4
#define FREE_MASK 2
#define EXIST_MASK 1

#define NEXT(prev, actual) (chunck*)((prev) ^ (actual))
#define PREV(next, actual) (chunck*)((next) ^ (actual))

#define CHUNCK_HEADER_SIZE (offsetof(struct s_chunck, xor_link)) // header size

#define chunk2mem(p)   ((void*)((char*)(p) + CHUNCK_HEADER_SIZE)) // convert a chunck ptr to a ptr on it's mem
#define mem2chunk(mem) ((chunck*)(((char*)(mem)) - CHUNCK_HEADER_SIZE)) // convert a mem ptr to a ptr on it's chunck

#define request2mem(size) (((size) + CHUNCK_ALIGN_MASK) & REVERSE_CUNCK_ALIGN_MASK)
#define request_oor(size) ((size) >= (-2*CHUNCK_HEADER_SIZE))

#define IS_EXIST(size_ptr) ((*(size_ptr)) & EXIST_MASK)
#define SET_EXIST(size_ptr) ((*(size_ptr)) |= EXIST_MASK)
#define UNSET_EXIST(size_ptr) ((*(size_ptr)) &= ~EXIST_MASK)

#define IS_FREE(size_ptr) (((*(size_ptr)) & FREE_MASK) >> 1)
#define SET_FREE(size_ptr) ((*(size_ptr)) |= FREE_MASK)
#define UNSET_FREE(size_ptr) ((*(size_ptr)) &= ~FREE_MASK)

#define IS_USED(size_ptr) (((*(size_ptr)) & USED_MASK) >> 2)
#define SET_USED(size_ptr) ((*(size_ptr)) |= USED_MASK)
#define UNSET_USED(size_ptr) ((*(size_ptr)) &= ~USED_MASK)

struct  s_chunck {
	size	mchunk_prev_size;  /* Size of previous chunk (if free).  */
	size	mchunk_size;       /* Size in bytes, including overhead. */

	link    xor_link; // only for free node
};

typedef struct s_chunck chunck;

#endif //_malloc_H_
