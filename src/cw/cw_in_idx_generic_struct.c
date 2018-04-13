
#include "capwap.h"
#include "msgset.h"
#include "ktv.h"
#include "log.h"
#include "cw.h"

int cw_in_idx_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	char key[CW_KTV_MAX_KEY_LEN];
	int idx;
	
	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}
	
	idx = cw_get_byte(elem_data);
	sprintf(key, handler->key, idx);

	cw_ktv_read_struct(params->conn->remote_cfg,handler->type,key,elem_data+1,elem_len-1);

	return CAPWAP_RESULT_SUCCESS;
}
