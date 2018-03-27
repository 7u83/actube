
#include "cw.h"
#include "dbg.h"


static int cw_put_radio_generic(struct cw_ElemHandler * handler, int idx, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	char key[CW_KTV_MAX_KEY_LEN];
	cw_KTV_t *elem, search;
	int len;
	uint8_t * d;
	
	len =0;
	
	sprintf(key,"radio/%d/%s",idx,handler->key);
	
	printf("Looking for key: %s\n",key);
	
	search.key=key;
	elem = mavl_get(params->conn->local_cfg, &search);
	
	if (elem==NULL){
		return 0;
	}
	
	/* Size for msg elem header depends on 
	   vendor specific payload */
	d = handler->vendor ? dst+10 : dst+4;
	
	/* put radio id */
	len += cw_put_byte(d+len,idx);
	
	len += ((const cw_Type_t*)(handler->type))->put(elem,d+len);
/*	l = len + cw_put_elem_hdr(dst, handler->id, len);*/
	
/*	if (handler->vendor)
		return len + cw_put_elem_vendor_hdr(dst, handler->vendor, handler->id, len);
*/
	return  len + cw_put_elem_hdr(dst, handler->id, len);
}


int cw_out_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
/*	char key[CW_KTV_MAX_KEY_LEN];*/
/*	cw_KTV_t *elem, search;*/
	int len,i,l;
	int radios;
	len =0;
	
	radios = cw_ktv_get_byte(params->conn->local_cfg,"wtp-descriptor/max-radios",0);
	printf("Must put radios: %d\n",radios);

	for(i=1;i<radios;i++){
		l = cw_put_radio_generic(handler,i,params,dst+len);
		printf("put radio %i, returns %d\n",i,l);
		cw_dbg_elem(DBG_ELEM_OUT,params->conn,params->msgdata->type,handler,dst,l);
		len+=l;
	}

	return len;
}
