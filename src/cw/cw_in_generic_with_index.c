#include "cw.h"
#include "dbg.h"

int cw_in_generic_with_index(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
	char key[CW_CFG_MAX_KEY_LEN];
	int idx;
	struct cw_Type * type = (struct cw_Type*)eh->type;

	idx = cw_get_byte(data);
	sprintf(key,"%s.%d",eh->key,idx);
//	cw_cfg_add(params->cfg,key,eh->type,NULL,data+1,len-1);
	type->read(params->cfg,key,data+1,len-1,eh->param);
	return 1;
}
