#include "mavl.h"

/**
 * Create an AVL tree
 * @param cmp pointer compare function
 * @param del pointer to delete function which is called when an element will be deletet
 * @return pointer to an #mavl struct
 */
struct mavl *mavl_create(int (*cmp) (const void *, const void *),
			       void (*del) (void *))
{
	struct mavl *t = malloc(sizeof(struct mavl));
	if (!t)
		return NULL;
	t->root = 0;
	t->count = 0;
	t->cmp = cmp;
	t->del = del;
	return t;
}


