#include "mlist.h"

mlistelem_t * mlist_append_ptr (mlist_t list, void * ptr)
{
	return mlist_append (list, &ptr);
}
