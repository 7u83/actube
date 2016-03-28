#include <errno.h>

#include "capwap.h"
#include "capwap_items.h"
#include "intavltree.h"
#include "log.h"
#include "dbg.h"
#include "sock.h"

int cw_in_check_disc_resp(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from)
{
	cw_action_in_t *mlist[20];
//	int n = cw_check_missing_mand(mlist, conn, a);

	//cw_dbg(DBG_INFO,"This response came from: %s",sock_addr2str(&conn->addr));


	/* if mandatory elements are missing, ignore this response */
/*	if (n && conn->strict_capwap) {
		cw_dbg_missing_mand(DBG_MSG_ERR, conn, mlist, n, a);
		cw_dbg(DBG_MSG_ERR,
		       "Ignoring Discovery Response from %s - missing mandatory elements.",
		       sock_addr2str(from));
		errno = EAGAIN;
		return -1;
	}
	if (n) {
		cw_dbg_missing_mand(DBG_RFC, conn, mlist, n, a);
	}
*/
	/*  we have all AC information in the incomming buffer */
	mbag_t discs;

	discs = mbag_get_mavl_c(conn->remote, CW_ITEM_DISCOVERIES,
					   mbag_i_create);

//mavl_del_all(discs);
//exit(0);

	if ( !discs ) {
		cw_log(LOG_ERR,"Can't allocate store for disc resp");
		errno = ENOMEM;
		return -1;
	}

	mbag_i_set_mavl(discs,discs->count,conn->incomming);

	conn->incomming = mbag_create();


	return 0;
}
