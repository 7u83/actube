
#include "cw.h"
#include "dbg.h"
#include "cfg.h"

int cw_ktv_idx_get_next(mavl_t ktv, const char *key, int n);

int cw_out_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	char key[CW_CFG_MAX_KEY_LEN];
	struct cw_Type * type;

	int len,i,l,start;
	int radios;
	len =0;
	
	radios = cw_cfg_get_byte(params->cfg,"wtp-descriptor/max-radios",0);
	for(i=0;i<radios;i++){
		l=0;

		type = (struct cw_Type*)handler->type;
		start = params->msgset->header_len(handler)+len;

		sprintf(key,"radio.%d/%s",i,handler->key);
		cw_dbg(DBG_X,"KEY: %s",key);

		l += cw_put_byte(dst+start+l,i);
		l += type->write(params->cfg_list, key,dst+start+l,handler->param);
	
		l = params->msgset->write_header(handler,dst+len,l);
		len+=l;

	}
	return len;
}

