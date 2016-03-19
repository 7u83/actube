
#include <errno.h>

#include "capwap.h"
#include "intavltree.h"
#include "dbg.h"
#include "log.h"
#include "capwap_items.h"

int cw_in_check_generic_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from)
{
	cw_action_in_t * mlist[60];

	/* Check for mandatory elements */
	int n = cw_check_missing_mand(mlist,conn,a);
	if (n) {
		if ( conn->strict_capwap ){
			cw_dbg_missing_mand(DBG_MSG_ERR,conn,mlist,n,a);
			return CW_RESULT_MISSING_MAND_ELEM;
		}
		cw_dbg_missing_mand(DBG_RFC,conn,mlist,n,a);
	}
	
	return 0;

}
