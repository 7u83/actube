#include "mavl.h"
#include "mavltypes.h"

void * mavl_get_ptr(mavl_t tree, void * search)
{
	void * result;
	
	result = mavl_get(tree,&search);
	if (result)
		return *(void**)result;
	return NULL;
}
