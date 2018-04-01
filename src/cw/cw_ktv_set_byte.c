#include "ktv.h"

cw_KTV_t * cw_ktv_set_byte(mavl_t ktv,const char * key, uint8_t byte)
{
	return cw_ktv_replace(ktv,key,CW_TYPE_BYTE,&byte,1);
}
