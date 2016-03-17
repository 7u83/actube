
#include "capwap.h"
#include "capwap_items.h"
#include "cw.h"

int cw_in_check_chng_state_evnt_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from)
{
	conn->capwap_state = CW_STATE_NONE;
	int rc = cw_in_check_generic_req(conn,a,data,len,from);
	if (rc==0)
		conn->capwap_state = CW_STATE_RUN;
	return rc;

}
