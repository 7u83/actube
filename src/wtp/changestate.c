#include "cw/capwap.h"

#include "cw/conn.h"
#include "cw/dbg.h"


#include "wtp_interface.h"
 

int changestate(struct cw_Conn * conn)
{


	/* Update operational states, so they will be included
	   in the change Change State Event Request message. */
/*//	cw_radio_update_oper_states(conn->radios,0);*/

/*//	mbag_set_dword(conn->outgoing,CW_ITEM_RESULT_CODE,0);*/

	cw_dbg(DBG_X,"CHANGE STATE");
	
	/* Change State ... */
	int rc = cw_send_request(conn,CAPWAP_MSG_CHANGE_STATE_EVENT_REQUEST);
	if ( !cw_result_is_ok(rc) ) {
		cw_strresult(rc);
		return 0;
	}

	return 1;
}

