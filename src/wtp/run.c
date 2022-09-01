#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include "cw/cw.h"
#include "cw/capwap.h"
#include "cw/conn.h"
#include "cw/log.h"
#include "cw/dtls.h"
#include "cw/sock.h"
#include "cw/cw_util.h"



#include "wtp_conf.h"


#include "cw/timer.h"
#include "wtp.h"


#include "cw/capwap.h"
#include "cw/conn.h"
#include "wtp_interface.h"

#include "cw/dbg.h"

int update =1;



static int update_cb(struct cw_ElemHandlerParams * params, struct cw_MsgCb_data *d)
{
//	cw_dbg(DBG_X," **** Configuration Update Request Received ***");
//	cw_cfg_dump(params->conn->global_cfg);
	cw_cfg_copy(params->cfg, params->conn->global_cfg,DBG_CFG_UPDATES,"GlobalCfg");
	cw_cfg_save(bootcfg.config_file, params->conn->global_cfg,
			"#\n# This file is igenerated  by WAT\n# If you edit this, your cahnges might be overwritten\n#\n");  
//	cw_dbg(DBG_X," **** Configuration Update Request Received Saved ***");
	return 0;
}




/*
static void update_radio(struct cw_Conn * conn, int rid, mbag_t radio_upd, mbag_t radio,mbag_t iresult )
{

	printf("Updating radio with rid %d\n",rid);
}
*/


/*
static void update_radios(struct cw_Conn * conn, mbag_t result)
{
	MAVLITER_DEFINE (it,conn->radios_upd);
	mavliter_foreach(&it){
		struct mbag_item * item = mavliter_get(&it);
		int rid = item->iid;
		mbag_t radio = mbag_i_get_mbag(conn->radios,rid,NULL);
		if (!radio){
			cw_dbg(DBG_ELEM_ERR,"Can't find radio with id %d",rid);
			continue;
		}

		mbag_t iresult = mbag_create();
		mbag_i_set_mbag(result,rid,iresult);		
		update_radio(conn,rid,item->data,radio,iresult);
	}

}
*/


int run(struct cw_Conn * conn)
{


	cw_conn_set_msg_cb(conn,CAPWAP_MSG_CONFIGURATION_UPDATE_REQUEST,update_cb);

	conn->capwap_state = CAPWAP_STATE_RUN;

	do {

		int echo_interval = cw_cfg_get_byte(conn->global_cfg,"capwap-timers/echo-interval",CAPWAP_ECHO_INTERVAL);
		
		/*mbag_get_word(conn->config,CW_ITEM_CAPWAP_TIMERS,CW_TIMERS)&0xff;*/
		
		time_t timer = cw_timer_start(echo_interval-1);
		int rc;

		


		while (!cw_timer_timeout(timer) && conn->capwap_state == CAPWAP_STATE_RUN) {
			cw_cfg_clear(conn->remote_cfg);
			rc = cw_read_messages(conn);
			if (rc < 0 && errno == EAGAIN) {
				continue;
			}
//printf("--------------------------------------------------------------------------------------\n");
			
			if ( !cw_result_is_ok(rc)) {
//printf("---------------------- cwrsult is not ok\n");				
				break;
			}

//printf("lalalala\n");			
//cw_dbg_ktv_dump(conn->remote_cfg,DBG_INFO,"KTV DUMP ----------------","Remote:", "DUMP done -------");

//			printf("Saving Config\n");
//			mavl_merge(conn->local_cfg,conn->remote_cfg);
//			cw_ktv_save(conn->local_cfg,"cisco.ktv");
//			clean_cfg(conn->remote_cfg);
			
//			cw_dbg(DBG_X,"We hav a message processed");
		}

		if (rc<0 && errno == EAGAIN){
			rc = cw_send_request(conn,CAPWAP_MSG_ECHO_REQUEST);
		
			if (!cw_result_is_ok(rc)) {
				cw_log(LOG_ERR,"Error in run state: %d %s",rc,cw_strrc(rc));
				break;
			}
			continue;
		}

		if (!cw_result_is_ok(rc)) {
			cw_log(LOG_ERR,"Error in run state: %d %s",rc,cw_strrc(rc));
			break;
		}

	} while (conn->capwap_state == CAPWAP_STATE_RUN);


/*
//      int rc = cw_send_request(conn,CW_MSG_CHANGE_STATE_EVENT_REQUEST);

//      if ( !cw_rcok(rc) ) {
//              cw_strresult(rc);
//      }
*/
	return 0;
}




