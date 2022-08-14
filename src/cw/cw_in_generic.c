
#include "capwap.h"
#include "msgset.h"
#include "val.h"
#include "log.h"
#include "dbg.h"

int cw_in_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	cw_Type_t * type;
	type = (cw_Type_t*)handler->type;
	char mkey[CW_CFG_MAX_KEY_LEN];
	const char *key;



	if (!type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	if (handler->mkkey != NULL){
		handler->mkkey(handler->key,elem_data,elem_len, mkey);
		key = mkey;
	}
	else{
		key = handler->key;
	}


	

/*	cw_dbg(DBG_X, "READ: %s / %s",type->name,key);*/
	type->read(params->cfg, key,elem_data,elem_len,handler->param);
	return CAPWAP_RESULT_SUCCESS;
}
