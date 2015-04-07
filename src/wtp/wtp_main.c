
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "capwap/capwap.h"
#include "capwap/capwap_items.h"
#include "capwap/conn.h"
#include "capwap/cw_log.h"
#include "capwap/dtls.h"

#include "wtp.h"
#include "wtp_conf.h"



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

	cw_dbg_opt_detail = DBG_DETAIL_ASC_DMP;


	dtls_init();

	the_conn = conn_create_noq(-1, NULL);
	struct conn *conn = the_conn;


	cw_register_actions_capwap_wtp(&capwap_actions);
	conn->actions = &capwap_actions;
	conn->remote = cw_itemstore_create();
	conn->local = cw_itemstore_create();

	cw_itemstore_t board_data = cw_itemstore_create();
	cw_itemstore_set_dword(board_data, CW_ITEM_WTP_BOARD_VENDOR, conf_vendor_id);


	cw_itemstore_set_bstrn(board_data, CW_ITEM_WTP_BOARD_MACADDRESS, conf_macaddress,
			       conf_macaddress_len);
	cw_itemstore_set_bstr16n(board_data, CW_ITEM_WTP_BOARD_SERIALNO, 
			bstr_data(conf_serial_no), bstr_len(conf_serial_no));




	cw_itemstore_set_avltree(conn->local, CW_ITEM_WTP_BOARD_DATA, board_data);

	discovery();


}


struct conn *get_conn()
{
	return the_conn;
}
