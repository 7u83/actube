
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "capwap/capwap.h"
#include "capwap/capwap_items.h"
#include "capwap/conn.h"
#include "capwap/log.h"
#include "capwap/dtls.h"
#include "capwap/acpriolist.h"

#include "wtp.h"
#include "wtp_conf.h"

#include "capwap/dbg.h"



/** The one and only connection object */
struct conn *the_conn;
struct cw_actiondef capwap_actions;



int main()
{

	wtpconf_preinit();

	if (!read_config("./wtp_uci.conf")) {
		return 1;
	}

	cw_dbg_opt_level = conf_dbg_level;

	wtpconf_init();

	cw_dbg_opt_display = DBG_DISP_ASC_DMP | DBG_DISP_COLORS;




	dtls_init();

	the_conn = conn_create_noq(-1, NULL);
	struct conn *conn = the_conn;


	cw_register_actions_capwap_wtp(&capwap_actions);
	////cw_register_actions_capwap_80211_wtp(&capwap_actions);

	conn->actions = &capwap_actions;
	conn->outgoing = cw_itemstore_create();
	conn->incomming = cw_itemstore_create();
	conn->local = cw_itemstore_create();

	cw_itemstore_t board_data = cw_itemstore_create();
	cw_itemstore_set_dword(board_data, CW_ITEM_WTP_BOARD_VENDOR, conf_vendor_id);


	cw_itemstore_set_bstrn(board_data, CW_ITEM_WTP_BOARD_MACADDRESS, conf_macaddress,
			       conf_macaddress_len);
	cw_itemstore_set_bstr16n(board_data, CW_ITEM_WTP_BOARD_SERIALNO, 
			bstr_data(conf_serial_no), bstr_len(conf_serial_no));




	cw_itemstore_set_avltree(conn->outgoing, CW_ITEM_WTP_BOARD_DATA, board_data);

	cw_acpriolist_t acprios = cw_acpriolist_create();
	cw_acpriolist_set(acprios,"Master AC",strlen("Master AC"),1);
	cw_acpriolist_set(acprios,"AC8new",strlen("AC8new"),12);

	cw_itemstore_set_avltree(conn->local,CW_ITEM_AC_PRIO_LIST,acprios);

	cw_itemstore_set_str(conn->local,CW_ITEM_LOCATION_DATA,"Berlin");
	cw_itemstore_set_str(conn->local,CW_ITEM_WTP_NAME,"WTP Tube");

	cw_itemstore_set_byte(conn->local,CW_ITEM_WTP_MAC_TYPE,0);
	cw_itemstore_set_byte(conn->local,CW_ITEM_WTP_FRAME_TUNNEL_MODE,0);



	discovery();
	join();
	image_update();


}


struct conn *get_conn()
{
	return the_conn;
}
