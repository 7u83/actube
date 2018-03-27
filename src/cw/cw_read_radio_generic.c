
#include "capwap.h"
#include "ktv.h"
#include "msgset.h"
#include "log.h"
#include "cw.h"


int cw_in_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	char key[CW_KTV_MAX_KEY_LEN];
	cw_KTV_t * result;
	int radio;

	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}
	
	radio=cw_get_byte(elem_data);
	sprintf(key,"radio/%d/%s",radio,handler->key);
	
	result = cw_ktv_add(params->conn->remote_cfg, key, 
				handler->type, elem_data+1,elem_len-1);
	
	params->elem=result;
	
	return CAPWAP_RESULT_SUCCESS;	
}
