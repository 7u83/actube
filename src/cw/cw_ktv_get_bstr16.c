#include "ktv.h"
#include "bstr.h"

bstr16_t cw_ktv_get_bstr16(mavl_t ktv,const char *key, bstr16_t def)
{
	cw_KTV_t * k;
	k = cw_ktv_get(ktv,key,CW_TYPE_STR);
	if (k != NULL){
		return bstr16_create(k->type->data(k),k->type->len(k));
	}
	return def;
}

