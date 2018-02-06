/*
** EPITECH PROJECT, 2018
** malloc
** File description:
** a malloc implementation in c
*/

#include "btree.h"

chunk	*rebase_tree(chunk *root)
{
	chunk	*save = root;
	chunk	*it = root->bigger;

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

chunk	*remove_from_tree(chunk *root, chunk *to_remove)
{
	chunk	*tmp;

	if (!to_remove || !root)
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

void	add_to_tree(chunk *to_add, chunk **free_tree)
{
	chunk	*save = *free_tree;
	chunk	*it = *free_tree;

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

chunk	*search_best_spot(size_t request, chunk *root_tree)
{
	chunk	*it = root_tree;
	chunk	*best = NULL;

	while (it && it->mchunk_size != request) {
		if (clean_size(it->mchunk_size) >= request &&
			(!best || clean_size(best->mchunk_size) >
			clean_size(it->mchunk_size))) {
			best = it;
			if (clean_size(best->mchunk_size) == request)
				return best;
		}
		it = move_tree(it, request);
	}
	return best;
}
