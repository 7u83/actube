#include "mavl.h"

void *mavl_add_ptr(mavl_t tree, void *ptr){
	void * result;
	result = mavl_add(tree,&ptr,NULL);
	if (result)
		return *(void**)result;
	return NULL;
}
