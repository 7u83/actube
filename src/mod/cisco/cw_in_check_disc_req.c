
#include "capwap.h"
#include "capwap_items.h"

#include "intavltree.h"
#include "log.h"
#include "dbg.h"
#include "sock.h"

int capwap_in_check_disc_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from)
{

	cw_action_in_t *mlist[120];
	int n = cw_check_missing_mand(mlist, conn, a);


	conn->capwap_state = CW_STATE_NONE;

	if (n && conn->strict_capwap) {
		cw_dbg_missing_mand(DBG_MSG_ERR, conn, mlist, n, a);
		/* if mandatory elements are missing, in strict 
		   mode send no discovery response */
		cw_dbg(DBG_MSG_ERR,
		       "Ignoring Discovery Request from %s - missing mandatory elements.",
		       sock_addr2str(from));
		return -1;
	}

	if ( n ) {
		/* put a warning here */
		cw_dbg_missing_mand(DBG_RFC, conn, mlist, n, a);
	}

		

	/* ok, send response */

	return 0;
}
