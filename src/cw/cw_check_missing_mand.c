
#include "capwap.h"
#include "dbg.h"

int cw_check_missing_mand(cw_action_in_t ** out, struct conn * conn, cw_action_in_t *a)
{


	cw_action_in_t as;

	as.capwap_state = a->capwap_state;
	as.msg_id = a->msg_id;
	as.vendor_id = 0;
	as.elem_id = 0;
	as.proto=0;

	DEFINE_AVLITER(it,conn->actions->in);
	int n=0;
	avliter_foreach_from(&it,&as) {
		cw_action_in_t * ai = avliter_get(&it);

		if (ai->msg_id != as.msg_id || ai->capwap_state != as.capwap_state) 
			break;
		if (!ai->mand)
			continue;

		void * rc = mavl_get(conn->mand,(void*)ai->item_id);
		if (!rc) {
			out[n++]=ai;
		}
	
	}
	return n;	
}


