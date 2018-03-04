#include "mavl.h"

void *mavl_add_ptr(mavl_t tree, void *ptr){
	mavldata_t data, *result;
	data.ptr = ptr;
	result = mavl_add(tree,&data);
	if (result)
		return result->ptr;
	return NULL;
}
