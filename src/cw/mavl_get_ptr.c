#include "mavl.h"

void * mavl_get_ptr(mavl_t tree, void * search)
{
	void * result;
	
	result = mavl_find(tree,&search);
	if (result)
		return *(void**)result;
	return NULL;
}
