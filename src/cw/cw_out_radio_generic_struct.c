
#include "cw.h"
#include "dbg.h"

int cw_ktv_idx_get_next(mavl_t ktv, const char *key, int n);

int cw_out_radio_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	cw_dbg(DBG_X,"KEY: %s",handler->key);
	stop();
/*
	int i,l, offset;

	uint8_t * cdst;
	
	cdst = dst;
	

	offset = params->msgset->header_len(handler);

	i=-1;
	while(1){
		char basekey[CW_CFG_MAX_KEY_LEN];
		cw_Val_t * result;
		
		i = cw_ktv_idx_get_next(params->cfg,"radio",i+1);
		if (i==-1)
			break;
		sprintf(basekey,"radio.%d/%s",i,handler->key);
		
		result = cw_ktv_base_exists(params->cfg,basekey);
		if (result == NULL){
			continue;
		}
			
		
		l=0;
		l+=cw_put_byte(cdst+offset+l,i);
		l+= cw_ktv_write_struct(params->cfg,NULL, handler->type,basekey,cdst+offset+l);
		

		cdst+=params->msgset->write_header(handler,cdst,l);
	}
	return cdst-dst;*/
}



