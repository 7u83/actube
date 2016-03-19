
#include "cw/capwap.h"
#include "cw/conn.h"
#include "cw/log.h"
#include "cw/mbag.h"
#include "cw/capwap_items.h"

#include "wtp_interface.h"
#include "cfg.h"

int configure()
{

	struct conn *conn = get_conn();

//	mbag_del_all(conn->incomming);
	conn->incomming=conn->config;
	mbag_del(conn->incomming,CW_ITEM_RESULT_CODE);

	mbag_set_str(conn->local,CW_ITEM_AC_NAME,"abc");


	int rc = cw_send_request(conn, CW_MSG_CONFIGURATION_STATUS_REQUEST);

	if (!cw_rcok(rc)) {
		if (rc > 0) {
			cw_log(LOG_ERR,
			       "Error sending Configuration Status Request to AC at %s, AC said: %d - %s.",
			       sock_addr2str(&conn->addr), rc, cw_strerror(rc));

		} else {
			cw_log(LOG_ERR,
			       "Error sending Configuration Status Request to AC at %s: %d - %s.",
			       sock_addr2str(&conn->addr), errno, cw_strerror(rc));
		}

		cfg_to_json();
		return 0;
	}

	return 1;
}
