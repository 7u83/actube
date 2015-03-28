#include "capwap/capwap.h"
#include "capwap/conn.h"
#include "wtp_interface.h"

int configure()
{
	struct conn * conn = get_conn();
	struct wtpinfo * wtpinfo = get_wtpinfo();

	struct radioinfo *rip = &(wtpinfo->radioinfo[0]);

	cw_prepare_configuration_status_request(conn,rip,wtpinfo);
	conn_send_request(conn);

exit(0);
}

