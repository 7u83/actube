
#include "cw.h"
#include "dbg.h"

int cw_ktv_idx_get_next(mavl_t ktv, const char *key, int n);

int cw_out_radio_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	int i,l, offset;
/*	int radios;*/
	uint8_t * cdst;
	
	cdst = dst;
	
/*	radios = cw_ktv_get_byte(params->conn->local_cfg,"wtp-descriptor/max-radios",0);*/

/*	int idx=0;
	while(1){
	
		idx = cw_ktv_idx_get_next(params->conn->local_cfg,"radio",idx);
		idx++;
	}
*/

	offset = params->conn->header_len(handler);
/*	for(i=0;i<radios;i++){*/
	i=-1;
	while(1){
		char basekey[CW_KTV_MAX_KEY_LEN];
		cw_KTV_t * result;
		
		i = cw_ktv_idx_get_next(params->conn->local_cfg,"radio",i+1);
		if (i==-1)
			break;
		sprintf(basekey,"radio.%d/%s",i,handler->key);
		
		result = cw_ktv_base_exists(params->conn->local_cfg,basekey);
		if (result == NULL){
			continue;
		}
			
		
		l=0;
		l+=cw_put_byte(cdst+offset+l,i);
		l+= cw_ktv_write_struct(params->conn->local_cfg,NULL, handler->type,basekey,cdst+offset+l);
		

		cdst+=params->conn->write_header(handler,cdst,l);
		

/*		l = cw_write_radio_element(handler,params,i,dst+len);
		cw_dbg_elem(DBG_ELEM_OUT,params->conn,params->msgdata->type,handler,dst,l);
		len+=l;*/
	}
	return cdst-dst;
}
