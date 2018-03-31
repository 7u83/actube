#include "ktv.h"

uint32_t cw_ktv_get_dword(mavl_t ktv,const char *key, uint32_t def)
{
	cw_KTV_t * k;
	k = cw_ktv_get(ktv,key,CW_TYPE_DWORD);
	if (k != NULL){
		return k->val.dword;
	}
	return def;
}
