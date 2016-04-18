


#include "action.h"
#include "cw.h"
#include "cw/dbg.h"


int cw_out_radio_generic(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
cw_dbg(DBG_X,"Radio Generic out %s",a->item_id);
	
	int l=0;
	MAVLITER_DEFINE(it,conn->radios_upd);
	mavliter_foreach(&it){
		struct mbag_item *radio = mavliter_get(&it);
		if ( radio->type != MBAG_MBAG ) {
			continue;
		}

		/* Size for msg elem header depends on 
		   vendor specific payload */
		int start = a->vendor_id ? 10 : 4;

		uint8_t * d = dst+l;



		struct mbag_item * item = mbag_get(radio->data,a->item_id);
		if (!item){
cw_dbg(DBG_X,"Not found! %s for rid %d",a->item_id,radio->iid);
			continue;
		}
		int len=0;
		len += cw_put_byte(d+start,radio->iid);

	cw_dbg(DBG_X, "Radio generic out '%s' fro rid %d",a->item_id,radio->iid);
		len += cw_put_mbag_item(d + start+1, item);

		if (a->vendor_id)
			l+= len + cw_put_elem_vendor_hdr(d, a->vendor_id, a->elem_id, len);
		else
			l += len + cw_put_elem_hdr(d, a->elem_id, len);



	}
	return l;

}
