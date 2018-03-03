#include "mavl.h"

void * mavl_get_ptr(mavl_t tree, void * search)
{
	mavldata_t search_data, *result;
	search_data.ptr=search;
	result = mavl_find(tree,&search_data);
	if (result)
		return result->ptr;
	return NULL;
}
