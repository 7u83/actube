#include "ktv.h"

char * cw_ktv_get_syst(mavl_t ktv,const char *key, void * def)
{
	cw_KTV_t * k;
	k = cw_ktv_get(ktv,key,CW_TYPE_STR);
	if (k != NULL){
		return k->val.ptr;
	}
	return def;
}
