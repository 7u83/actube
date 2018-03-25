#include <string.h>

#include "mlist.h"

struct mlistelem * mlist_replace(mlist_t list, void *data)
{
	struct mlistelem *e;
	
	e = mlist_get(list,data);
	if (e == NULL)
		return NULL;

	memcpy(mlistelem_dataptr(e), data,list->data_size);
	return e;

}
