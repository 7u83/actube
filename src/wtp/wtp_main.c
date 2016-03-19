
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "cw/capwap.h"
#include "cw/cipwap.h"
#include "cw/capwap_items.h"
#include "cw/conn.h"
#include "cw/log.h"
#include "cw/dtls.h"
#include "cw/acpriolist.h"
//#include "cw/capwap_80211.h"
#include "cw/radio.h"

#include "cw/mod.h"


#include "wtp.h"
#include "wtp_conf.h"

#include "cw/dbg.h"
#include "cw/mavl.h"

#include "jsmn.h"

#include "cfg.h"


#include "../mod/modload.h"



/** The one and only connection object */
struct conn *the_conn;
struct cw_actiondef capwap_actions;



int handle_update_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
		      int len, struct sockaddr *from)
{
	MAVLITER_DEFINE(it, conn->incomming);

	mavliter_foreach(&it) {
		mbag_item_t *item = mavliter_get(&it);

//		printf("MBAG ITEM GOT: %s\n",item->id);
		if (item->id == CW_ITEM_LOCATION_DATA) {
			printf("Location Data %s\n",mbag_get_str(conn->incomming,CW_ITEM_LOCATION_DATA,"ups"));
			
		}

	}
	cw_dbg(DBG_INFO, "Saving configuration ...");
	cfg_to_json();

//	exit(0);
	return 0;

}


const char *t = CW_ITEM_WTP_NAME;

#include <signal.h>



void update_reboot_stats(struct conn * conn, int cause)
{


	mbag_t rs = mbag_get_mbag(conn->config,CW_ITEM_WTP_REBOOT_STATISTICS,NULL);

	switch (cause){
		case CW_REBOOT_TYPE_NOT_SUPPORTED:
			break;
		case CW_REBOOT_TYPE_AC_INITIATED:
			mbag_inc_word(rs,CW_ITEM_REBOOT_AC_INITIATED_COUNT,1);
			break;
		case CW_REBOOT_TYPE_OTHER_FAILURE:
			mbag_inc_word(rs,CW_ITEM_REBOOT_OTHER_FAILURE_COUNT,1);
			break;
			
		

	}

	mbag_inc_word(rs,CW_ITEM_REBOOT_COUNT,1);
	mbag_set_byte(rs,CW_ITEM_REBOOT_LAST_FAILURE_TYPE,cause);
	cfg_to_json();
}


static void sig_handler(int sig)
{
	struct conn * conn = the_conn; //get_conn();

	update_reboot_stats(conn, CW_REBOOT_TYPE_OTHER_FAILURE);
	exit(0);
}


int main()
{
	signal (SIGINT, sig_handler);



	wtpconf_preinit();

	if (!read_config("./wtp_uci.conf")) {
		return 1;
	}
//      cw_dbg_opt_level = conf_dbg_level;

	wtpconf_init();

	cw_dbg_opt_display = DBG_DISP_ASC_DMP | DBG_DISP_COLORS;

	dtls_init();

	the_conn = conn_create_noq(-1, NULL);
	struct conn *conn = the_conn;

	conn->radios = mbag_i_create();
	mbag_i_set_mbag(conn->radios,0,mbag_create());
	mbag_i_set_mbag(conn->radios,1,mbag_create());
//      mbag_set_mbag(conn->radios,0xff,mbag_create());


	mbag_t r;
	r  = mbag_i_get_mbag(conn->radios,0,NULL);
	mbag_set_dword(r,CW_RADIO_TYPE,1);
	r  = mbag_i_get_mbag(conn->radios,1,NULL);
	mbag_set_dword(r,CW_RADIO_TYPE,2);
//      r  = mbag_get_mbag(conn->radios,1,NULL);
//      mbag_set_dword(r,CW_RADIO_TYPE,1);


//      cw_register_actions_cipwap_wtp(&capwap_actions);
//      cw_register_actions_capwap_80211_wtp(&capwap_actions);
//
#define CWMOD "cisco"
#define CWBIND "cisco"
//#define CWMOD "capwap"
//#define CWBIND "capwap80211"


	struct mod_wtp *mod = modload_wtp(CWMOD);
	if (!mod) {
		printf("Can't load mod capwap\n");
		exit(0);
	}
	mod->init();

	mod->register_actions(&capwap_actions,MOD_MODE_CAPWAP);
	mod = modload_wtp(CWBIND);
	if (!mod) {
		printf("Can't load mod capwap80211\n");
		exit(0);
	}

	printf("add capwap80211\n");
	int rc = mod->register_actions(&capwap_actions,MOD_MODE_BINDINGS);

	printf("Out reg = %d\n",rc);

	conn->detected = 1;
	conn->dtls_verify_peer=0;
	conn->dtls_mtu = 22180;



/*
	MAVLITER_DEFINE(it,capwap_actions.strelem);
	mavliter_foreach(&it){
		struct cw_str *s = mavliter_get(&it);

	}
*/

	////cw_register_actions_capwap_80211_wtp(&capwap_actions);

	conn->actions = &capwap_actions;
	conn->outgoing = mbag_create();
	conn->incomming = mbag_create();
	conn->local = mbag_create();

//	conn->base_rmac = get_base_rmac();

//	conn->capwap_mode = CW_MODE_CAPWAP;
	//conn->capwap_mode = CW_MODE_CISCO;

	the_conn->strict_capwap = 0;


	conn->config = mbag_create();

	cfg_from_json(conn);
	setup_conf(conn);
	mod_init_config(mod,conn->config);
	cfg_to_json();

//	mbag_t board_data = mbag_create();
//	mbag_set_dword(board_data, CW_ITEM_WTP_BOARD_VENDOR, conf_vendor_id);


//	mbag_set_bstrn(board_data, CW_ITEM_WTP_BOARD_MACADDRESS, conf_macaddress,
//		       conf_macaddress_len);
//	mbag_set_bstr16n(board_data, CW_ITEM_WTP_BOARD_SERIALNO,
//			 bstr_data(conf_serial_no), bstr_len(conf_serial_no));

//	mbag_set_bstr16n(board_data, CW_ITEM_WTP_BOARD_MODELNO, bstr_data(conf_model_no),
//			 bstr_len(conf_model_no));



//	mbag_set_mbag(conn->config, CW_ITEM_WTP_BOARD_DATA, board_data);




//      mbag_set_bstrv(conn->config, CW_ITEM_WTP_SOFTWARE_VERSION, sw);

//	printf("Board_data %p\n", board_data);

	mbag_t mb = mbag_get_mbag(conn->config, CW_ITEM_WTP_BOARD_DATA, NULL);
	printf("mbag %p\n", mb);


	cw_acpriolist_t acprios = cw_acpriolist_create();
	cw_acpriolist_set(acprios, "Master AC", strlen("Master AC"), 1);
	cw_acpriolist_set(acprios, "AC8new", strlen("AC8new"), 12);


      //mbag_set_str(conn->config,CW_ITEM_LOCATION_DATA,"Berlin");
      //mbag_set_str(conn->config,CW_ITEM_WTP_NAME,"WTP Tube");

	mbag_set_byte(conn->local, CW_ITEM_WTP_MAC_TYPE, 0);
	mbag_set_byte(conn->local, CW_ITEM_WTP_FRAME_TUNNEL_MODE, 0);

	conn->wbid=1;




cw_set_msg_end_callback(conn,CW_STATE_RUN,CW_MSG_CONFIGURATION_UPDATE_REQUEST,handle_update_req);
//cw_set_msg_end_callback(conn,CW_STATE_CONFIGURE,CW_MSG_CONFIGURATION_STATUS_RESPONSE,handle_update_req);



	if (!discovery())
		return -1;
	if (!join())
		return -1;

//	mavl_del_all(conn->incomming);
//	conn->incomming = conn->config;
	if (!configure())
		return -1;


	cfg_to_json();

	changestate();


	run();

	//image_update();

	return 0;
}


struct conn *get_conn()
{
	return the_conn;
}
