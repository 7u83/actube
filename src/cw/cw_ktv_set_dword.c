#include "val.h"

cw_Val_t * cw_ktv_set_dword(mavl_t ktv,const char * key, uint32_t dword)
{
	return cw_ktv_replace(ktv,key,CW_TYPE_DWORD,NULL,(void*)&dword,4);
}
