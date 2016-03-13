
#include <errno.h>

#include "capwap.h"
#include "intavltree.h"
#include "dbg.h"
#include "log.h"
#include "capwap_items.h"

int cw_in_check_generic_resp(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from)
{


	/* Check if the message contains a result code and
	   if it indicates an errror */
	mbag_item_t * result = mbag_get(conn->incomming,CW_ITEM_RESULT_CODE);
	
	if  (result ) {
		if (!cw_rcok(result->dword)){
			return result->dword;
		}
	}

	cw_action_in_t * mlist[60];

	/* Check for mandatory elements */
	int n = cw_check_missing_mand(mlist,conn,a);
	if (n && conn->strict_capwap) {
		cw_dbg_missing_mand(DBG_MSG_ERR,conn,mlist,n,a);
		errno=EAGAIN;
		return -1; /* CW_RESULT_MISSING_MAND_ELEM */
	}
	if (n){
		cw_dbg_missing_mand(DBG_RFC,conn,mlist,n,a);
	}

	/* if we hava a result code, return it */
	if ( result ) {
		return result->dword;
	}

	return 0;
}
