#include "mlist.h"

struct mlistelem * mlist_get(mlist_t list, const void *data){
	struct mlistelem * elem;
	mlist_foreach(elem,list){
		void *tdata = mlistelem_dataptr(elem);
		if (list->cmp(tdata,data)==0){
			return tdata;
		}
	}
	return NULL;
}
