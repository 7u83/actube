


#include "action.h"
#include "cw.h"


int cw_out_radio_generic(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	int l=0;
	MAVLITER_DEFINE(it,conn->radios_upd);
	mavliter_foreach(&it){
		struct mbag_item *i = mavliter_get(&it);
		if ( i->type != MBAG_MBAG ) {
			continue;
		}
		cw_out_generic(conn,a,dst);

	}
	return l;

}
