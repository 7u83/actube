#include "val.h"

char * cw_ktv_get_str(mavl_t ktv,const char *key, char * def)
{
	cw_Val_t * k;
	k = cw_ktv_get(ktv,key,CW_TYPE_STR);
	if (k != NULL){
		return k->val.ptr;
	}
	return def;
}
