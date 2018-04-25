
#include "capwap.h"
#include "msgset.h"
#include "ktv.h"
#include "log.h"

int cw_in_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	cw_KTV_t * result;

	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}
	
	result = cw_ktv_add(params->conn->remote_cfg, handler->key, 
				handler->type,NULL, elem_data,elem_len);
	
	params->elem=result;
	
	return CAPWAP_RESULT_SUCCESS;
}
