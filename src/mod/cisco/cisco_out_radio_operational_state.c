#include "cw/cw.h"
#include "cw/dbg.h"
#include "cw/capwap_items.h"


int cisco_out_radio_administrative_states(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	int l=0;
	MAVLITER_DEFINE(it,conn->radios_upd);
	mavliter_foreach(&it){
		struct mbag_item *i = mavliter_get(&it);
		if ( i->type != MBAG_MBAG ) {
			continue;
		}

		int state = mbag_get_byte(i->data,CW_RADIOITEM_ADMIN_STATE,CW_RADIO_ADMIN_STATE_DISABLED);
		l+=cw_put_elem_radio_administrative_state(dst+l,i->iid,i->data);
//		l+=cw_put_elem_radio_operational_state(dst+l,i->iid,state<<8,0);

	}
	return l;
}


