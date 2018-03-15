#include "ktv.h"
#include "mavl.h"

cw_KTV_t * cw_ktv_get(mavl_t ktv, const char *key, const cw_Type_t * type)
{
	cw_KTV_t search, *result;
	/* we can safely cast from const char * to char *, because
	 * we never will use the search varaiable to store ktv values */
	search.key=(char*)key;
	
	result = mavl_get(ktv,&search);
	if (result == NULL){
		return NULL;
	}
	if (type == NULL){
		return result;
	}
	if (strcmp(type->name,result->type->name)==0){
		return result;
	}
	
	return NULL;
}
