
#include "ktv.h"
#include "mavl.h"

/**
 * @brief Get a ktv value from a ktv store
 * @param ktv ktv store
 * @param key key to search for
 * @param type type to match
 * @return A pointer to a #cw_KTV_t element, found in the ktv store or
 * NULL if no element with matching key/type is found.
 */
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
