#include "ktv.h"

uint16_t cw_ktv_get_word(mavl_t ktv,const char *key, uint16_t def)
{
	cw_KTV_t * k;
	k = cw_ktv_get(ktv,key,CW_TYPE_WORD);
	if (k != NULL){
		return k->val.word;
	}
	return def;
}
