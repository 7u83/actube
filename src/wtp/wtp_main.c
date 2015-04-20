
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "capwap/capwap.h"
#include "capwap/cipwap.h"
#include "capwap/capwap_items.h"
#include "capwap/conn.h"
#include "capwap/log.h"
#include "capwap/dtls.h"
#include "capwap/acpriolist.h"
#include "capwap/capwap_80211.h"
#include "capwap/radio.h"

#include "wtp.h"
#include "wtp_conf.h"

#include "capwap/dbg.h"
#include "capwap/mavl.h"

#include "jsmn.h"








/** The one and only connection object */
struct conn *the_conn;
struct cw_actiondef capwap_actions;


bstr_t get_base_rmac()
{
//	static	uint8_t rm[8]={0x00,0x4a,0x99,0x02,0xfa,0xc0};

	static     uint8_t rm[8]={0x00,0x3a,0x99,0x02,0xfa,0xc0};
	return bstr_create(rm,6);
}


int handle_update_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
                         int len,struct sockaddr *from)
{
	printf("There was an config update request\n");
	MAVLITER_DEFINE(it,conn->incomming);
	printf("Here are the results\n");

	mavliter_foreach(&it){
		mbag_item_t * item = mavliter_get(&it);

		printf("MBAG ITEM GOT: %d\n",item->id);
		if (item->id == CW_ITEM_WTP_NAME) {
			printf("Yea! WTP NAME\n");
			
			printf("The name is %.*s\n",bstr16_len(item->data),bstr16_data(item->data));

		}

	}

	cfg_json_save();	
	return 0;

}




int main()
{




	wtpconf_preinit();

	if (!read_config("./wtp_uci.conf")) {
		return 1;
	}

//	cw_dbg_opt_level = conf_dbg_level;

	wtpconf_init();

	cw_dbg_opt_display = DBG_DISP_ASC_DMP | DBG_DISP_COLORS;



mbag_t b = mbag_create();

mbag_set_byte(b,1,99);
mbag_set_avltree(b,2,mbag_create());
mavl_destroy(b);


	dtls_init();

	the_conn = conn_create_noq(-1, NULL);
	struct conn *conn = the_conn;

	conn->radios = mbag_create();
	mbag_set_mbag(conn->radios,0,mbag_create());
	mbag_set_mbag(conn->radios,1,mbag_create());
//	mbag_set_mbag(conn->radios,0xff,mbag_create());
	

	mbag_t r;
	r  = mbag_get_mbag(conn->radios,0,NULL);
	mbag_set_dword(r,CW_RADIO_TYPE,1);
	r  = mbag_get_mbag(conn->radios,1,NULL);
	mbag_set_dword(r,CW_RADIO_TYPE,2);
//	r  = mbag_get_mbag(conn->radios,1,NULL);
//	mbag_set_dword(r,CW_RADIO_TYPE,1);




	cw_register_actions_cipwap_wtp(&capwap_actions);
	cw_register_actions_capwap_80211_wtp(&capwap_actions);

	////cw_register_actions_capwap_80211_wtp(&capwap_actions);

	conn->actions = &capwap_actions;
	conn->outgoing = mbag_create();
	conn->incomming = mbag_create();
	conn->local = mbag_create();
	conn->base_rmac=get_base_rmac();
	conn->capwap_mode = CW_MODE_CISCO;

conn->config=mbag_create();

	setup_conf(conn);


	mbag_t board_data = mbag_create();
	mbag_set_dword(board_data, CW_ITEM_WTP_BOARD_VENDOR, conf_vendor_id);


	mbag_set_bstrn(board_data, CW_ITEM_WTP_BOARD_MACADDRESS, conf_macaddress,
			       conf_macaddress_len);
	mbag_set_bstr16n(board_data, CW_ITEM_WTP_BOARD_SERIALNO, 
			bstr_data(conf_serial_no), bstr_len(conf_serial_no));

	
	mbag_set_avltree(conn->outgoing, CW_ITEM_WTP_BOARD_DATA, board_data);

	cw_acpriolist_t acprios = cw_acpriolist_create();
	cw_acpriolist_set(acprios,"Master AC",strlen("Master AC"),1);
	cw_acpriolist_set(acprios,"AC8new",strlen("AC8new"),12);

	mbag_set_avltree(conn->local,CW_ITEM_AC_PRIO_LIST,acprios);

	mbag_set_str(conn->local,CW_ITEM_LOCATION_DATA,"Berlin");
//	mbag_set_str(conn->local,CW_ITEM_WTP_NAME,"WTP Tube");

	mbag_set_byte(conn->local,CW_ITEM_WTP_MAC_TYPE,0);
	mbag_set_byte(conn->local,CW_ITEM_WTP_FRAME_TUNNEL_MODE,0);




cw_set_msg_end_callback(conn,CW_STATE_RUN,CW_MSG_CONFIGURATION_UPDATE_REQUEST,handle_update_req);



	the_conn->strict_capwap=0;
	discovery();
	join();
	configure();
	changestate();

mavl_destroy(conn->incomming);
conn->incomming=conn->config;

	run();

	//image_update();


}


struct conn *get_conn()
{
	return the_conn;
}
