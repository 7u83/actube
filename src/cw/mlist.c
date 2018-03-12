#include <stdlib.h>
#include <string.h>

#include "mlist.h"



struct mlistelem * mlist_prepend(mlist_t l, void *data){
	
}




struct mlistelem *mlist_find(mlist_t l, struct mlistelem *start, void *data)
{
	/*
	struct mlistelem *e;
	if (start)
		e = start;
	else
		e = l->first;

	while (e) {
		if (l->cmp(e->data, data) == 0)
			return e;
		e = e->next;
	}

	return NULL;
	 */
}


struct mlistelem * xmlist_replace(mlist_t l, struct mlistelem *start, void *data)
{
/*
	struct mlistelem *e;
	if (start)
		e = start;
	else
		e = l->first;

	struct mlistelem * f = mlist_find(l,e,data);
	if (!f)
		return NULL;

	f->data = data;
	return f;
*/
}
