#include <string.h>
#include <stdio.h>

#include "mlist.h"

struct mlistelem * mlist_delete(mlist_t list, void *data)
{
	struct mlistelem *e;
	
	mlist_foreach(e,list){
		void *tdata = mlistelem_dataptr(e);
		if (list->cmp(tdata,data)==0){
			break;
		}
	}

	if (e == NULL)
		return NULL;

	if (e->prev==NULL && e->next==NULL){
		if (list->del)
			list->del(e);
		free(e);
		list->count=0;
		list->first=NULL;
		list->last=NULL;
		return NULL;
	}
	
	if (e==list->first){
		list->first=e->next;
		e->next->prev=NULL;
		if (list->del)
			list->del(e);
		list->count--;
		free(e);
		return NULL;
		
	}
	
	if (e==list->last){
		list->last=e->prev;
		e->prev->next=NULL;
		if (list->del)
			list->del(e);
		list->count--;
		free(e);
		return NULL;
	
	}

	e->next->prev=e->prev;
	e->prev->next=e->next;
	if (list->del)
		list->del(e);
	list->count--;
	free(e);
	return NULL;

}
