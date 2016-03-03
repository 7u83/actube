
#include "capwap.h"
#include "intavltree.h"
#include "dbg.h"
#include "log.h"
#include "capwap_items.h"

int cw_in_check_chng_state_evnt_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from)
{
	conn->capwap_state = CW_STATE_RUN;
	return 0;	
}
