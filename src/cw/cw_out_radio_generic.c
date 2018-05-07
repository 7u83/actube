
#include "cw.h"
#include "dbg.h"

int cw_ktv_idx_get_next(mavl_t ktv, const char *key, int n);

int cw_out_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	int len,i,l;
	int radios;
	len =0;
	
/*	int idx=0;*/
	
/*	while(1){
		char key[CW_KTV_MAX_KEY_LEN];
		sprintf(key,"radio.%d",idx);
		idx = cw_ktv_idx_get_next(params->conn->local_cfg,key,idx);
		idx++;
	}
*/	
	radios = cw_ktv_get_byte(params->conn->local_cfg,"wtp-descriptor/max-radios",0);

	for(i=0;i<radios;i++){
		l = cw_write_radio_element(handler,params,i,dst+len);
		cw_dbg_elem(DBG_ELEM_OUT,params->conn,params->msgdata->type,handler,dst,l);
		len+=l;
	}
	return len;
}

