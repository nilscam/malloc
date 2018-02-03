/*
* Created by nils on 2/3/18.
*/

#ifndef _btree_H_
#define _btree_H_

#include "../malloc.h"

#define move_tree(it, size_to_add) ((clean_size((it)->mchunk_size) >\
					clean_size((size_to_add))) ? \
					(it)->smaller : (it)->bigger)

/* chunk comparaisons */
#define cmp_chunk_grt(chk1, chk2) (clean_size((chk1)->mchunk_size) > \
					clean_size((chk2)->mchunk_size))
#define cmp_chunk_lrt(chk1, chk2) (clean_size((chk1)->mchunk_size) < \
					clean_size((chk2)->mchunk_size))
#define cmp_chunk_lrt_oe(chk1, chk2) (clean_size((chk1)->mchunk_size) <= \
					clean_size((chk2)->mchunk_size))
/* */

chunk   *remove_from_tree(chunk *, chunk *);
void    add_to_tree(chunk *, chunk **);
chunk   *search_best_spot(size_t, chunk *);
chunk   *rebase_tree(chunk *);

#endif /* _btree_H_ */
