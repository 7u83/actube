
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
#include "cw/capwap80211_items.h"

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

#include "cw/dot11.h"
#include "cw/format.h"

#include "cw/capwap80211_types.h"

int main()
{

	signal (SIGINT, sig_handler);

	wtpconf_preinit();

	if (!read_config("./wtp_uci.conf")) {
		return 1;
	}
//	cw_dbg_opt_level = conf_dbg_level;


	if (!wtpconf_init()){
		return 1;
	};

	cw_dbg_opt_display = DBG_DISP_ASC_DMP | DBG_DISP_COLORS;

	dtls_init();


	the_conn = conn_create_noq(-1, NULL);
	struct conn *conn = the_conn;

	conn->radios = mbag_i_create();
	conn->radios_upd=mbag_i_create(); 

	mbag_i_set_mbag(conn->radios,0,mbag_create());
	mbag_i_set_mbag(conn->radios_upd,0,mbag_create());



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

	int rc = mod->register_actions(&capwap_actions,MOD_MODE_BINDINGS);

	conn->detected = 1;
	conn->dtls_verify_peer=0;
	conn->dtls_mtu = 12000;


	conn->actions = &capwap_actions;

	conn->outgoing = mbag_create();
	conn->incomming = mbag_create();
	conn->local = mbag_create();
	conn->config = mbag_create();

	the_conn->strict_capwap = 0;

	cfg_from_json(conn);
	setup_conf(conn);

	mbag_t r;
//	r  = mbag_i_get_mbag(conn->radios,0,NULL);
	r = conn->radios;
	MAVLITER_DEFINE(it,r);
	mavliter_foreach(&it){
		struct mbag_item *i=mavliter_get(&it);
		printf("RID = %d\n",i->iid);
		printf("DATA: %p\n",i->data);
		mbag_t radio= (mbag_t)i->data;
		struct mbag_item *mri = mbag_get(radio,CW_RADIOITEM80211_WTP_RADIO_INFORMATION);
		
	if (!mri){
printf("Setting to 8 %p %p\n",mri,r);
		mbag_set_dword(radio,CW_RADIOITEM80211_WTP_RADIO_INFORMATION,1);
	}
	else{
		printf("MRI %p\n",mri);
	}

		
	}
	



	mod_init_config(mod,conn->config);
	cfg_to_json();

	mbag_t mb = mbag_get_mbag(conn->config, CW_ITEM_WTP_BOARD_DATA, NULL);
	printf("mbag %p\n", mb);


	cw_acpriolist_t acprios = cw_acpriolist_create();
	cw_acpriolist_set(acprios, "Master AC", strlen("Master AC"), 1);
	cw_acpriolist_set(acprios, "AC8new", strlen("AC8new"), 12);



	mbag_set_byte(conn->local, CW_ITEM_WTP_MAC_TYPE, CW_WTP_MAC_TYPE_SPLIT);
	mbag_set_byte(conn->local, CW_ITEM_WTP_FRAME_TUNNEL_MODE, CW_WTP_FRAME_TUNNEL_MODE_E);
	conn->wbid=1;




//	cw_set_msg_end_callback(conn->actions,CW_STATE_RUN,CW_MSG_CONFIGURATION_UPDATE_REQUEST,handle_update_req);


	if (!discovery())
		return -1;
	if (!join())
		return -1;

	if (!configure())
		return -1;

	cw_dbg(DBG_X,"Saveing config 0");

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
