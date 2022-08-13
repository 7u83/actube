#include "cw.h"
#include "dbg.h"

int cw_in_generic_with_index(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
	char key[CW_KTV_MAX_KEY_LEN];
	int idx;

cw_dbg(DBG_X,"Fix cw_in_generic_with_index");
stop();	
	idx = cw_get_byte(data);
	sprintf(key,"%s.%d",eh->key,idx);
//	cw_cfg_add(params->cfg,key,eh->type,NULL,data+1,len-1);

	return 1;
}
