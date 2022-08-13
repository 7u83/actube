
#include "capwap.h"
#include "val.h"
#include "msgset.h"
#include "log.h"
#include "cw.h"

#include "dbg.h"

int cw_in_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	char key[CW_CFG_MAX_KEY_LEN];
	int radio;
	struct cw_Type *type;
	type = (struct cw_Type*)handler->type;

	if (!type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	
	
	radio=cw_get_byte(elem_data);
	sprintf(key,"radio.%d/%s",radio,handler->key);
	
	/*cw_cfg_set_val(params->cfg, key, 
				handler->type, handler->param, elem_data+1,elem_len-1);*/
	
	type->read(params->cfg, key,elem_data+1,elem_len-1,handler->param);
	return CAPWAP_RESULT_SUCCESS;	
}
