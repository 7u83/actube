


#include "cw.h"
#include "capwap_items.h"

#include "dbg.h"
#include "log.h"

/*
int cw_put_item(uint8_t * dst, struct mbag_item *item)
{
	if (MBAG_STR == item->type ){
		return cw_put_data(dst, item->data, strlen((char *) item->data));
	}

	if (MBAG_BYTE == item->type){
		return cw_put_byte(dst, item->byte);
	}
	if (MBAG_WORD == item->type){
		return cw_put_word(dst, item->word);
	}
	if (MBAG_DWORD == item->type){
		return cw_put_dword(dst, item->dword);
	}
	if (MBAG_BSTR  == item->type) {
		return cw_put_bstr(dst, item->data);
	}

	if ( MBAG_BSTR16 == item->type)
		return cw_put_bstr16(dst,item->data);

	if (MBAG_VENDORSTR == item->type)
	{
		int l=0;
		l+=cw_put_dword(dst, bstrv_get_vendor_id(item->data));
		l+=cw_put_data(dst+4, bstrv_data(item->data),bstrv_len(item->data));
		return l;
	}
	cw_log(LOG_ERR,"No method to put items of type %d",item->type);


	return 0;
}
*/


int cw_out_generic(struct conn *conn, struct cw_action_out *a, uint8_t * dst)	// ,struct mbag_item * item) 
{


	/* Get the item to put */
	struct mbag_item *item = NULL;
	if (a->get) {
		item = a->get(conn, a);
	}


	/* Size for msg elem header depends on 
	   vendor specific payload */
	int start = a->vendor_id ? 10 : 4;


	int len;
	if (!item) {
		const char *vendor="";
		if ( a->vendor_id ) {
			vendor=cw_strvendor(a->vendor_id);
		}
		if (a->mand) {
			cw_log(LOG_ERR,
			       "Can't put mandatory element %s%d - (%s) into %s. No value found.",
				vendor,
			       a->elem_id, cw_strelemp(conn->actions, a->elem_id)
			       , cw_strmsg(a->msg_id)
			    );
		}
		else{
			cw_dbg(DBG_WARN,"No output for element %s%d -(%s) in %s. Item %s not found.",
				vendor,
			       a->elem_id, cw_strelemp(conn->actions, a->elem_id)
			       , cw_strmsg(a->msg_id),a->item_id);

		}
		return 0;
	} else {
		len = cw_put_mbag_item(dst + start, item);
	}


	if (a->vendor_id)
		return len + cw_put_elem_vendor_hdr(dst, a->vendor_id, a->elem_id, len);

	return len + cw_put_elem_hdr(dst, a->elem_id, len);
}




