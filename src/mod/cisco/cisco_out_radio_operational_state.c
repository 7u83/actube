#include "cw/cw.h"
#include "cw/dbg.h"
#include "cw/capwap_items.h"


int cisco_out_radio_operational_state(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{

	int l=0;
	MAVLITER_DEFINE(it,conn->radios_upd);
	mavliter_foreach(&it){
		struct mbag_item *i = mavliter_get(&it);
		if ( i->type != MBAG_MBAG ) {
			continue;
		}

		int state = mbag_get_word(i->data,CW_RADIOITEM_OPER_STATE,CW_RADIO_OPER_STATE_DISABLED<<8);
		int oper_state = (state >> 8) & 0xff;
		int oper_cause = state & 0xff;

		int oper_state_t = oper_state;
		if (oper_state == CW_RADIO_OPER_STATE_ENABLED)
			oper_state_t = CW_RADIO_OPER_STATE_ENABLED_7;
		if (oper_state == CW_RADIO_OPER_STATE_DISABLED)
			oper_state_t = CW_RADIO_OPER_STATE_DISABLED_7;

		cw_dbg(DBG_X,"Translated oper state for output from %d to %d",oper_state, oper_state_t);

		l+=cw_put_elem_radio_operational_state(dst+l,i->iid,oper_state_t, oper_cause);


	}
	return l;
}


