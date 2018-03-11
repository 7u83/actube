#include <string.h>

#include "mlist.h"

struct mlistelem *mlist_append(mlist_t l, void *data)
{
	struct mlistelem *e;

	e = malloc(sizeof(struct mlistelem) + l->data_size);

	if (e == NULL)
		return NULL;

	memcpy(mlistelem_dataptr(e), data, l->data_size);

	if (l->first == NULL) {
		l->first = e;
		l->last = e;
		e->prev=NULL;
		e->next=NULL;
		l->count++;
		return e;
	}
	
	l->last->next=e;
	e->prev=l->last;
	e->next=NULL;
	l->last=e;
	l->count++;
	return e;
}
