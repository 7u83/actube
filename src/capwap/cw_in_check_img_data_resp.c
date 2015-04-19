#include <stdio.h>
#include <errno.h>

#include "capwap.h"
#include "dbg.h"
#include "log.h"
#include "capwap_items.h"


int cw_in_check_img_data_resp(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from)
{
	cw_action_in_t * mlist[60];

	/* Check for mandatory elements */
	int n = cw_check_missing_mand(mlist,conn,a);
	if (n) {
		cw_dbg_missing_mand(DBG_ELEM,conn,mlist,n,a);
		conn->capwap_state=CW_STATE_JOIN;
		errno=EAGAIN;
		return -1; //CW_RESULT_MISSING_MAND_ELEM;
	}

	

	mbag_item_t * iresult = mbag_get(conn->incomming,	CW_ITEM_RESULT_CODE);

	if ( iresult ) {
		return iresult->dword;
	}

	return 0;
	
}
