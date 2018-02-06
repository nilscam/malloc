/*
** EPITECH PROJECT, 2018
** malloc
** File description:
** a malloc implementation in c
*/

#include "malloc.h"

void	combine_chunk(chunk *to_combine, chunk **free_tree)
{
	chunk	*second = NEXT(to_combine);
	size_t	new_size = combine_size_chunk(to_combine, second);

	*free_tree = remove_from_tree(*free_tree, to_combine);
	*free_tree = remove_from_tree(*free_tree, second);

	to_combine->mchunk_size = new_size;
	second = NEXT(to_combine);
	second->mchunk_prev_size = to_combine->mchunk_size;
	add_to_tree(to_combine, free_tree);
}

void	discharge(void *mem, chunk **free_tree)
{
	chunk	*chunck = mem2chunk(mem);
	chunk	*next = NEXT(chunck);

	SET_FREE(chunck->mchunk_size);
	add_to_tree(chunck, free_tree);
	if (IS_EXIST(chunck->mchunk_size)) {
		SET_FREE(next->mchunk_prev_size);
		if (IS_FREE(next->mchunk_size)) {
			combine_chunk(chunck, free_tree);
		}
	}
	if (IS_EXIST(chunck->mchunk_prev_size &&
		IS_FREE(chunck->mchunk_prev_size))) {
		chunck = PREV(chunck);
		combine_chunk(chunck, free_tree);
	}
	if (reduce_heap(chunck) == SUCCESS)
		*free_tree = remove_from_tree(*free_tree, chunck);
}
