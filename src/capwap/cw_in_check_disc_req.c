
#include "capwap.h"
#include "capwap_items.h"

#include "intavltree.h"
#include "cw_log.h"
#include "dbg.h"
#include "sock.h"

int cw_in_check_disc_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len)
{

	cw_action_in_t *mlist[20];
	int n = cw_check_missing_mand(mlist, conn, a);

	cw_dbg_missing_mand(DBG_ELEM, conn, mlist, n, a);

	conn->capwap_state = CW_STATE_NONE;

	/* if mandatory elements are missing send no discovery response */
	if (n) {
		cw_dbg(DBG_CW_INFO,
		       "Ignoring Discovery Request from %s - missing mandatory elements.",
		       sock_addr2str(&conn->addr));
		return -1;
	}

		

	/* ok, send response */

	return 0;
}
