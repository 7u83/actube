
#include "capwap.h"
#include "msgset.h"
#include "val.h"
#include "log.h"

int cw_in_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	cw_Val_t * result;
	cw_Val_t val;
	cw_Type_t * type;
	int n;
	char strval[2048];
	
	memset(&val,0,sizeof(cw_Val_t));


	type = (cw_Type_t*)handler->type;

	if (!type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	type->read(params->local_cfg, handler->key,elem_data,elem_len,handler->param);


//	exit(0);

/*	if (!result){
		cw_log(LOG_ERR, "Can't create kvstore element for key %s of type %s: %s",
				handler->key,type->name, strerror(errno));
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}
*/

	


//	n = type->to_str(&val,strval,2048);

//	printf("SETTING: %s: %s (%d)\n",handler->key,strval,n);


//	result = cw_ktv_add(params->remote_cfg, handler->key, 
//				handler->type,NULL, elem_data,elem_len);
//	params->elem=result;
	return CAPWAP_RESULT_SUCCESS;
}
