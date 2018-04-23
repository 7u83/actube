
#include "capwap.h"
#include "msgset.h"
#include "ktv.h"
#include "log.h"

int cw_in_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	const char * key;
	char tmpkey[CW_KTV_MAX_KEY_LEN];
	
	if (handler->mkkey != NULL){
		handler->mkkey(key,elem_data,elem_len, tmpkey);
		key = tmpkey;
	}
	else{
		key = handler->key;
	}

/*	int (*mkkey)(struct cw_ElemHandler * handler, char *dst, struct cw_ElemHandlerParams * params, 
		uint8_t*data, int len);
*/

	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	cw_ktv_read_struct(params->conn->remote_cfg,handler->type,key,elem_data,elem_len);

	return CAPWAP_RESULT_SUCCESS;
}
