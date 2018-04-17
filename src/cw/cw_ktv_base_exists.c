#include "ktv.h"
/**
 * @file 
 * @description Implementation of cw_ktv_base_exisits
 */


/**
 * @brief Check if elements with e certain baskey can be found in 
 * in ktvstore.
 * @param ktvstore ktvstore to search in
 * @param basekey basekey to search for
 * @return The first ktv element belonging to the base eky, 
 * otherwise NULL if no element was found. 
 * 
 */
cw_KTV_t * cw_ktv_base_exists(mavl_t ktvstore, const char *basekey)
{
	cw_KTV_t * result, search;
	search.key=(char*)basekey;
	result = mavl_get_first(ktvstore,&search);
	if (result == NULL)
		return NULL;
	
	if (strncmp(result->key,basekey,strlen(basekey))==0)
		return result;

	return NULL;
} 
