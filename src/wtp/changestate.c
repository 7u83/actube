#include "capwap/capwap.h"
#include "capwap/conn.h"
#include "wtp_interface.h"


int changestate()
{


	struct conn * conn = get_conn();
	int rc = cw_send_request(conn,CW_MSG_CHANGE_STATE_EVENT_REQUEST);

	if ( !cw_rcok(rc) ) {
		cw_strresult(rc);
		return 0;
	}

	return 1;
}

