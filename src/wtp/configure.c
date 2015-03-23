#include "capwap/conn.h"
#include "wtp_interface.h"

int configure()
{
	struct conn * conn = get_conn();
	struct wtpinfo * wtpinfo = get_wtpinfo();

	cw_prepare_configuration_status_request(conn,wtpinfo);
	conn_send_request(conn);

}

