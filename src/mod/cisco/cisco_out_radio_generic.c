

#include "cw/cw.h"
#include "cw/dbg.h"
#include "cw/cfg.h"

#include "mod_cisco.h"


int cisco_out_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	
	int len,i,l;
	int radios;
	len =0;
	
	radios = cw_cfg_get_byte_l(params->cfg_list,"wtp-descriptor/max-radios",0);
cw_dbg(DBG_X,"NUM RADIOS: %d",radios);
	for(i=0;i<radios+0;i++){
		l = cw_write_radio_element(handler,params,i,dst+len);
//		cw_dbg_elem(DBG_ELEM_OUT,NULL,params->msgdata->type,handler,dst+len,l);
		len+=l;
	}
	return len;
}



int cisco_out_radio_info(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	if (! (params->msgdata->type & 1) ){
		int start = params->msgset->header_len(handler);
		cw_set_byte(dst+start,0);
		cw_set_dword(dst+start+1,7);
		return params->msgset->write_header(handler,dst,5);
	}
	return cw_out_radio_generic(handler,params,dst);	
}




