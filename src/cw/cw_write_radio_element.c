#include "cw.h"


int cw_write_radio_element(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params, int idx 
			, uint8_t * dst)
{
	return 0;
	stop();

	char key[CW_KTV_MAX_KEY_LEN];
	cw_Val_t *elem, search;
	int len;
	uint8_t * d;
	
	len =0;
	sprintf(key,"radio.%d/%s",idx,handler->key);
	
	printf("Looking for readio key: %s\n",key);
	
	search.key=key;
	elem = mavl_get(params->cfg, &search);
	
	if (elem==NULL){
		printf("Nothing found\n");
		return 0;
	}
	
	/* Size for msg elem header depends on 
	   vendor specific payload */
	d = handler->vendor ? dst+10 : dst+4;
	
	/* put radio id */

	printf("Putting put byte index: %d\n",idx);

	len += cw_put_byte(d+len,idx);
	
	len += ((const cw_Type_t*)(handler->type))->put(elem,d+len);
/*	l = len + cw_put_elem_hdr(dst, handler->id, len);*/
	
	if (handler->vendor)
		return len + cw_put_elem_vendor_hdr(dst, handler->vendor, handler->id, len);

	return  len + cw_put_elem_hdr(dst, handler->id, len);
}

