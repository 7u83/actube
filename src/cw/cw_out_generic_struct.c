#include "capwap.h"
#include "msgset.h"
#include "val.h"
#include "log.h"
#include "cw.h"

#include "dbg.h"


int cw_out_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	cw_dbg(DBG_X,"Key: %s",handler->key);
	stop();


	int start;
	int len;
	cw_Val_t search, *result;

	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return 0;
	}
	
	search.key = (char*)handler->key;
	result = mavl_get_first(params->cfg,&search);
	if (result == NULL ){
		if (params->elemdata->mand)
			cw_log(LOG_ERR,"Can't put mandatory message element %s, no data available",handler->name);
		return 0;
	}
	
	if (strncmp(result->key,handler->key, strlen(handler->key))!=0){
		if (params->elemdata->mand)
			cw_log(LOG_ERR,"Can't put mandatory message element %s, no data available",handler->name);
		return 0;
	}

	start = params->msgset->header_len(handler);

	len = cw_ktv_write_struct(params->cfg,
		params->cfg,
		handler->type,handler->key,dst+start);
	
	return params->msgset->write_header(handler,dst,len);

}
