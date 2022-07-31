#include "val.h"

void * cw_ktv_get_sysptr(mavl_t ktv,const char *key, void * def)
{
	cw_Val_t * k;
	k = cw_ktv_get(ktv,key,CW_TYPE_SYSPTR);
	if (k != NULL){
		return k->val.ptr;
	}
	return def;
}
