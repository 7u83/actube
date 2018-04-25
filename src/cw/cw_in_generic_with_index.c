#include "cw.h"

int cw_in_generic_with_index(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
	char key[CW_KTV_MAX_KEY_LEN];
	int idx;
	
	idx = cw_get_byte(data);
	sprintf(key,"%s.%d",eh->key,idx);
	cw_ktv_add(params->conn->remote_cfg,key,eh->type,NULL,data+1,len-1);
	return 1;
}
