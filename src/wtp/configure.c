#include <stdlib.h> //Tube

#include "capwap/capwap.h"
#include "capwap/conn.h"
#include "wtp_interface.h"

int configure()
{

	struct conn * conn = get_conn();
	cw_send_request(conn,CW_MSG_CONFIGURATION_STATUS_REQUEST);



/*	struct wtpinfo * wtpinfo = get_wtpinfo();

	struct radioinfo *rip = &(wtpinfo->radioinfo[0]);

	cw_prepare_configuration_status_request(conn,rip,wtpinfo);
	struct cwrmsg * cwrmsg = conn_send_request(conn);

	int rc = cw_readmsg_configuration_status_response(cwrmsg->msgelems,cwrmsg->msgelems_len);

	printf("Chage Sate\n");
	cw_prepare_change_state_event_request(conn,rip,wtpinfo);
	cwrmsg = conn_send_request(conn);


	printf("Got change resp %p\n",cwrmsg);
*/

}

