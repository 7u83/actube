#include "cw/capwap.h"

#include "cw/action.h"
#include "cw/cw.h"

int cisco_out_capwap_up(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{
	return cw_put_elem_radio_operational_state(dst,0xff,CW_RADIO_OPER_STATE_ENABLED_7,CW_RADIO_OPER_CAUSE_NORMAL);
}

