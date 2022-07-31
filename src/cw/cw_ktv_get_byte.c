#include "val.h"

uint8_t cw_ktv_get_byte(mavl_t ktv,const char *key, uint8_t def)
{
	cw_Val_t * k;
	k = cw_ktv_get(ktv,key,CW_TYPE_BYTE);
	if (k != NULL){
		return k->val.byte;
	}
	return def;
}
