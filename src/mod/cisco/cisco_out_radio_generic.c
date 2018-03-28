

#include "cw/cw.h"
#include "cw/dbg.h"

#include "mod_cisco.h"


int cisco_out_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	int len,i,l;
	int radios;
	len =0;
	
	radios = cw_ktv_get_byte(params->conn->local_cfg,"wtp-descriptor/max-radios",0);

	for(i=0;i<radios+0;i++){
		l = cw_write_radio_element(handler,params,i,dst+len);
		cw_dbg_elem(DBG_ELEM_OUT,params->conn,params->msgdata->type,handler,dst,l);
		len+=l;
	}
	return len;
}

