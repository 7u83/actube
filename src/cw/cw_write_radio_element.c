#include "cw.h"
#include "dbg.h"
#include "log.h"



int cw_write_radio_element(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params, int idx 
			, uint8_t * dst)
{

	char key[CW_CFG_MAX_KEY_LEN];
//	cw_Val_t *elem;
	int len;
	uint8_t * d;
	
	len =0;
	sprintf(key,"radio.%d/%s",idx,handler->key);
	
//	printf("Looking for readio key: %s\n",key);
	
//	search.key=key;
//	elem = mavl_get(params->cfg, &search);
//	elem = cw_cfg_get_val_l(params->cfg_list, key, handler->type);
	
	
//	if (elem==NULL){
//		printf("Nothing found\n");
//		return 0;
//	}
	
	/* Size for msg elem header depends on 
	   vendor specific payload */
//	d = handler->vendor ? dst+10 : dst+4;
	d = dst + params->msgset->header_len(handler);

	/* put radio id */

	len = cw_generic_write_l(params->cfg_list,((const cw_Type_t*)(handler->type)), 
			key, d, handler->param);
	if (len==-1){
		const char *vendor="";
		if ( handler->vendor ) {
			vendor=cw_strvendor(handler->vendor);
		}
		if ( params->elemdata->mand) {
			cw_log(LOG_ERR,
			       "Can't put mandatory element %s %d-(%s) into %s. No value for '%s' found.",
				vendor, handler->id, handler->name, params->msgdata->name
			       , handler->key
			    );
		}
		return 0;
	}

	

	printf("Putting put byte index: %d\n",idx);

	len += cw_put_byte(d+len,idx);
	
//	len += ((const cw_Type_t*)(handler->type))->put(elem,d+len);
/*	l = len + cw_put_elem_hdr(dst, handler->id, len);*/

	return params->msgset->write_header(handler,dst,len);

/*
	if (handler->vendor)
		return len + cw_put_elem_vendor_hdr(dst, handler->vendor, handler->id, len);

	return  len + cw_put_elem_hdr(dst, handler->id, len);*/

}

