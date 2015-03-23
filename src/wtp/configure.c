#include "capwap/conn.h"
#include "wtp_interface.h"

int configure()
{
sleep(10);
	struct conn * conn = get_conn();
	struct wtpinfo * wtpinfo = get_wtpinfo();

	cw_prepare_configuration_status_request(conn,wtpinfo);
	conn_send_request(conn);

exit(0);
}

