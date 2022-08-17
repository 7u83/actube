


#include "cw/capwap.h"
#include "cw/conn.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "wtp_interface.h"
#include "cfg.h"


static int config_cb(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len)
{
	struct cw_DiscoveryResults *results = (struct cw_DiscoveryResults *)params->conn->data;
	cw_dbg(DBG_X,"Configurations status response received");
	cw_cfg_dump(params->cfg);
	return 0;
}



int configure(struct cw_Conn * conn)
{
	char sockbuff[SOCK_ADDR_BUFSIZE];
	
//	cw_dbg_ktv_dump(conn->local_cfg,DBG_INFO,"KTV DUMP ----------------","LOCAL:", "DUMP done -------");
	
	cw_conn_set_msg_cb(conn,CAPWAP_MSG_CONFIGURATION_STATUS_RESPONSE,config_cb);
	
	int rc;
	rc = cw_send_request(conn, CAPWAP_MSG_CONFIGURATION_STATUS_REQUEST);

	if (!cw_result_is_ok(rc)) {
		if (rc > 0) {
			cw_log(LOG_ERR,
			       "Error sending Configuration Status Request to AC at %s, AC said: %d - %s.",
			       sock_addr2str(&conn->addr,sockbuff), rc, cw_strerror(rc));

		} else {
			cw_log(LOG_ERR,
			       "Error sending Configuration Status Request to AC at %s: %d - %s.",
			       sock_addr2str(&conn->addr,sockbuff), errno, cw_strerror(rc));
		}

		/*cfg_to_json();*/
		return 0;
	}
	
//	cw_dbg_ktv_dump(conn->remote_cfg,DBG_INFO,"Config ***","CFG: ", "End config ***");

//	cw_ktv_set_byte(conn->remote_cfg,"
/*exit(0);*/
	return 1;
}
