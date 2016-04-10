
#include "capwap.h"
#include "intavltree.h"
#include "dbg.h"
#include "log.h"
#include "capwap_items.h"
#include "format.h"
#include "connlist.h"

int cw_in_check_join_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from)
{

	cw_action_in_t * mlist[60];

	/* Check for mandatory elements */
	int n = cw_check_missing_mand(mlist,conn,a);
	if (n) {
		if ( conn->strict_capwap ){
			cw_dbg_missing_mand(DBG_MSG_ERR,conn,mlist,n,a);
			conn->capwap_state=CW_STATE_JOIN;
			return CW_RESULT_MISSING_MAND_ELEM;
		}
		cw_dbg_missing_mand(DBG_RFC,conn,mlist,n,a);
	}

	connlist_lock(conn->connlist);
	struct conn *cc = connlist_get_by_session_id(conn->connlist,conn);
	if (!cc){
		connlist_add_by_session_id(conn->connlist,conn);
	}
	connlist_unlock(conn->connlist);
	if (cc){
		cw_dbg(DBG_ELEM_ERR,"Session already in use %s",format_bin2hex(conn->session_id,16));
		conn->capwap_state=CW_STATE_JOIN;
		return CW_RESULT_JOIN_FAILURE_SESSION_ALREADY_IN_USE;
	}
	
	/* set result code to ok and change to configure state */
	mbag_set_dword(conn->outgoing,CW_ITEM_RESULT_CODE,0);
	conn->capwap_state = CW_STATE_CONFIGURE;

	return 0;
}
