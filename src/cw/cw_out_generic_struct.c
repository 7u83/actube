#include "capwap.h"
#include "msgset.h"
#include "ktv.h"
#include "log.h"
#include "cw.h"

int cw_out_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	int start;
	int len;
	cw_KTV_t search, *result;

	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return 0;
	}
	
	search.key = (char*)handler->key;
	result = mavl_get_first(params->local_cfg,&search);
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

	start = cw_header_len(handler);

	len = cw_ktv_write_struct(params->local_cfg,
		params->default_cfg,
		handler->type,handler->key,dst+start);
	
	return cw_write_header(handler,dst,len);

}
