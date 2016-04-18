#include "cw/capwap.h"
#include "cw/capwap_items.h"
#include "cw/conn.h"
#include "cw/radio.h"

#include "wtp_interface.h"

int changestate()
{


	struct conn * conn = get_conn();

	/* Update operational states, so they will be included
	   in the change Change State Event Request message. */
//	cw_radio_update_oper_states(conn->radios,0);

	mbag_set_dword(conn->outgoing,CW_ITEM_RESULT_CODE,0);


	
	/* Change State ... */
	int rc = cw_send_request(conn,CW_MSG_CHANGE_STATE_EVENT_REQUEST);
	if ( !cw_rcok(rc) ) {
		cw_strresult(rc);
		return 0;
	}

	return 1;
}

