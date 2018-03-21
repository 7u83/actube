#include "ktv.h"

cw_KTV_t * cw_ktv_idx_get(mavl_t ktv, const char *key, int idx, const cw_Type_t * type)
{
	char ikey[CW_KTV_MAX_KEY_LEN];
	sprintf(ikey,"%s.%d",key,idx);
	return cw_ktv_get(ktv,ikey,type);
}
