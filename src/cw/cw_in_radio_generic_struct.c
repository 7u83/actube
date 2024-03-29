
#include "capwap.h"
#include "msgset.h"
#include "val.h"
#include "log.h"
#include "cw.h"

#include "dbg.h"

int cw_in_radio_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{

	stop();

	char key[CW_CFG_MAX_KEY_LEN];
	int radio;
	
	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}
	
	radio = cw_get_byte(elem_data);
	sprintf(key,"radio.%d/%s",radio,handler->key);

	cw_ktv_read_struct(params->cfg,handler->type,key,elem_data+1,elem_len-1);

	/*params->elem=result;*/
	
	return CAPWAP_RESULT_SUCCESS;
}
