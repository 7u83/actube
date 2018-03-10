#include "msget.h"
#include "kvt.h"
#include "log.h"

int cw_in_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len){
/*	mavldata_t data, *result;
	char str[30];
*/

	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return 0;
	}
	cw_kvt_add(params->conn->remote_cfg, handler->key, handler->type, elem_data,elem_len);
	
	
/*
	result = handler->type->get(&data,elem_data,elem_len);
	result->kv.key = strdup(handler->key);
	
	handler->type->to_str(result,str,30);
	mavl_add(conn->remote_cfg, result);
*/	 
	return 0;
}
