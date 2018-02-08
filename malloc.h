/*
** EPITECH PROJECT, 2018
** malloc
** File description:
** a malloc implementation in c
*/

#ifndef _malloc_H_
#define _malloc_H_

#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <zconf.h>
#include <pthread.h>

struct  s_chunk {
	size_t	mchunk_prev_size;
	size_t	mchunk_size;

	struct s_chunk	*smaller;
	struct s_chunk	*bigger;
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
		((long int)((char*)(chk)))))
#define PREV(chk) ((chunk*)(((long int)((char*)(chk))) - \
		clean_size((chk)->mchunk_prev_size)))
/* */

/* ptr/size conversion */
#define chunk2mem(p) ((void*)((char*)(p) + CHUNK_HEADER_SIZE))
#define mem2chunk(mem) ((chunk*)(((char*)(mem)) - CHUNK_HEADER_SIZE))
#define schunk2smem(s) ((s) - CHUNK_HEADER_SIZE)
#define smem2schunk(s) ((s) + CHUCK_HEADER_SIZE)
#define clean_size(size) ((size) & REVERSE_CHUNK_ALIGN_MASK)
/* */

/* logic calcul */
#define request2mem(size) (clean_size((size) + CHUNK_ALIGN_MASK) + \
			CHUNK_HEADER_SIZE)
#define request2chunk(size) (((request2mem(size)) > MIN_CHUNK_SIZE) ? \
			request2mem(size) : MIN_CHUNK_SIZE)
#define request_oor(size) ((size) >= (4000000000))
#define combine_size_chunk(first, second) ((clean_size((first)->mchunk_size))\
				+ (clean_size((second)->mchunk_size))\
				+ ((second)->mchunk_size & CHUNK_ALIGN_MASK))
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

#include "btree/btree.h"
#include "manage_heap/manage_heap.h"

/* system */
void	*sbrk(intptr_t titi);
int	getpagesize(void);
/* */

/* DEBUG */
void	dump_memory(chunk *);
void	put_addr(void *);
void	put_nbr(unsigned long int);
/* */

void	clear_mem(void *);
void	*allocate(size_t, chunk **);
void	discharge(void *, chunk **);
mbool	reduce_heap(chunk *);
void	*increase_heap(size_t);
void	show_alloc_mem();
void	unlock_thread();
void	lock_thread();

#endif /*_malloc_H_*/
