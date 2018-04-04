#include "ktv.h"

uint8_t cw_ktv_get_bool(mavl_t ktv,const char *key, uint8_t def)
{
	cw_KTV_t * k;
	k = cw_ktv_get(ktv,key,CW_TYPE_BOOL);
	if (k != NULL){
		return k->val.boolean;
	}
	return def;
}
