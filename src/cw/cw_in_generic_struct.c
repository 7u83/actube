
#include "capwap.h"
#include "msgset.h"
#include "val.h"
#include "log.h"
#include "dbg.h"

int cw_in_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	cw_dbg(DBG_X,"STRUCT KEY: %s",handler->key);
	stop();
/*	const char * key;
	char tmpkey[CW_CFG_MAX_KEY_LEN];

	
	if (handler->mkkey != NULL){
		handler->mkkey(key,elem_data,elem_len, tmpkey);
		key = tmpkey;
	}
	else{
		key = handler->key;
	}

//printf("CW_IN_GENERIC STRUCT: %s\n",key);	


	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	cw_ktv_read_struct(params->cfg,handler->type,key,elem_data,elem_len);
*/
	return CAPWAP_RESULT_SUCCESS;
}
