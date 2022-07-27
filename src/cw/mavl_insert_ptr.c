#include "mavl.h"
#include "mavltypes.h"

void *mavl_insert_ptr(mavl_t tree, const void *ptr){
	void * result;
	result = mavl_insert(tree,&ptr,NULL);
	if (result)
		return *(void**)result;
	return NULL;
}
