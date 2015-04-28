#include "capwap/capwap.h"
#include "capwap/conn.h"
#include "capwap/radio.h"

#include "wtp_interface.h"

int changestate()
{


	struct conn * conn = get_conn();

	/* Update operational states, so they will be included
	   in the change Change State Event Request message. */
	cw_radio_update_oper_states(conn->radios,0);

	
	/* Change State ... */
	int rc = cw_send_request(conn,CW_MSG_CHANGE_STATE_EVENT_REQUEST);
	if ( !cw_rcok(rc) ) {
		cw_strresult(rc);
		return 0;
	}

	return 1;
}

