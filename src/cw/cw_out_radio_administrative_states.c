#include "cw.h"
#include "dbg.h"


int cw_out_radio_administrative_states(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	cw_dbg(DBG_X,"Radio admin updater");



	int l=0;
	MAVLITER_DEFINE(it,conn->radios_upd);
	mavliter_foreach(&it){
		struct mbag_item *i = mavliter_get(&it);
		if ( i->type != MBAG_MBAG ) {
			continue;
		}
		l+=cw_put_elem_radio_administrative_state(dst+l,i->iid,i->data);

	}
	return l;
}


