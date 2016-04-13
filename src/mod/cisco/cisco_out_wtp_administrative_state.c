#include "cw/capwap.h"

#include "cw/action.h"
#include "cw/cw.h"

int cisco_out_wtp_administrative_state(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	int state = mbag_get_byte(conn->outgoing, a->item_id,1);
	return cw_put_elem_radio_administrative_state(dst,0xff,state);
}

