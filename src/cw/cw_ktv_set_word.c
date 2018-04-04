#include "ktv.h"

cw_KTV_t * cw_ktv_set_word(mavl_t ktv,const char * key, uint16_t word)
{
	return cw_ktv_replace(ktv,key,CW_TYPE_WORD,(void*)&word,2);
}
