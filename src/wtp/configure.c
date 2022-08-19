


#include "cw/capwap.h"
#include "cw/conn.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "wtp_interface.h"
#include "wtp.h"


static int config_cb(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len)
{
	cw_dbg(DBG_X,"*** Configurations Status Response received ****");
	cw_cfg_copy(params->cfg, params->conn->global_cfg);
	cw_cfg_save(bootcfg.cfgfilename, params->conn->global_cfg,
			"#\n# This file is igenerated  by WAT\n# If you edit this, your cahnges might be overwritten\n#\n");  
	cw_dbg(DBG_X,"*** Cnofig Saved ***");

	return 0;
}



int configure(struct cw_Conn * conn)
{
	char sockbuff[SOCK_ADDR_BUFSIZE];
	
	
	cw_conn_set_msg_cb(conn,CAPWAP_MSG_CONFIGURATION_STATUS_RESPONSE,config_cb);
	
	int rc;
	cw_cfg_copy(conn->global_cfg,conn->update_cfg);
	rc = cw_send_request(conn, CAPWAP_MSG_CONFIGURATION_STATUS_REQUEST);
	cw_cfg_clear(conn->update_cfg);

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
	
	return 1;
}
