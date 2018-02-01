//
// Created by nils on 1/29/18.
//

#include "malloc.h"

chunk   *rebase_tree(chunk *root)
{
	chunk   *save = root;
	chunk   *it = root->bigger;

	if (!it->smaller) {
		save->bigger = NULL;
		return it;
	}
	while (it->smaller) {
		save = it;
		it = it->smaller;
	}
	save->smaller = it->bigger;
	return it;
}

chunk   *remove_from_tree(chunk *root, chunk *to_remove)
{
	chunk   *tmp;

	if (!to_remove)
		return root;
	if (cmp_chunk_grt(root, to_remove))
		root->smaller = remove_from_tree(root->smaller, to_remove);
	else if (cmp_chunk_lrt_oe(root, to_remove) && root != to_remove)
		root->bigger = remove_from_tree(root->bigger, to_remove);
	else {
		if (!root->smaller)
			return root->bigger;
		if (!root->bigger)
			return root->smaller;
		tmp = rebase_tree(root);
		tmp->smaller = root->smaller;
		tmp->bigger = root->bigger;
		return tmp;
	}
	return root;
}

void    add_to_tree(chunk *to_add, chunk **free_tree)
{
	chunk   *save = *free_tree;
	chunk   *it = *free_tree;

	to_add->bigger = NULL;
	to_add->smaller = NULL;
	if (!it) {
		*free_tree = to_add;
	} else {
		while (it) {
			save = it;
			it = move_tree(it, to_add->mchunk_size);
		}
		if (cmp_chunk_grt(save, to_add))
			save->smaller = to_add;
		else
			save->bigger = to_add;
	}
}

void    combine_chunk(chunk *to_combine, chunk **free_tree)
{
	chunk   *second = NEXT(to_combine);
	size_t  new_size = combine_size_chunk(to_combine, second);

	*free_tree = remove_from_tree(*free_tree, to_combine);

	printf("here: %s, %d\n", __FILE__, __LINE__);
	*free_tree = remove_from_tree(*free_tree, second);
	printf("here: %s, %d\n", __FILE__, __LINE__);

	to_combine->mchunk_size = new_size;
	if (reduce_heap(to_combine) == FAILURE)
		add_to_tree(to_combine, free_tree);
}

void    discharge(void *mem, chunk **free_tree)
{
	chunk   *chunck = mem2chunk(mem);
	chunk   *tmp = NEXT(chunck);

	SET_FREE(chunck->mchunk_size);
	add_to_tree(chunck, free_tree);
	print_header(chunck);
	if (IS_EXIST(chunck->mchunk_size) && IS_FREE(tmp->mchunk_size)) {
		combine_chunk(chunck, free_tree);
	}
	if (IS_EXIST(chunck->mchunk_prev_size && IS_FREE(chunck->mchunk_prev_size))) {
		combine_chunk(PREV(chunck), free_tree);
	}
}
