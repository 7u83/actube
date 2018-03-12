#include "mlist.h"

void mlist_destroy(mlist_t l){
	mlistelem_t * elem, * next;
	elem = l->first;
	while (elem){
		if (l->del != NULL){
			l->del(mlistelem_dataptr(elem));
		}
		next = elem->next;
		free(elem);
		elem=next;
	}
	free(l);
}
