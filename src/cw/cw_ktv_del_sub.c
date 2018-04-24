

#include "ktv.h"

void cw_ktv_del_sub(mavl_t ktvstore, const char *basekey)
{
	cw_KTV_t * result, search;

	
	while (1){
		search.key=(char*)basekey;
		result = mavl_get_first(ktvstore,&search);
	
		if (result == NULL)
			return;
	
		if (strncmp(result->key,basekey,strlen(basekey))!=0)
			break;
		search.key = result->key;
		mavl_del(ktvstore,&search);
	}
}
