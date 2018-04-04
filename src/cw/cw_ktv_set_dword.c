#include "ktv.h"

cw_KTV_t * cw_ktv_set_dword(mavl_t ktv,const char * key, uint32_t dword)
{
	return cw_ktv_replace(ktv,key,CW_TYPE_DWORD,(void*)&dword,4);
}
