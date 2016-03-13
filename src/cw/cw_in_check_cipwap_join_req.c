
#include "capwap.h"
#include "intavltree.h"
#include "dbg.h"
#include "log.h"
#include "capwap_items.h"
#include "bstr.h"
#include "sock.h" //tube

#include "vendors.h"

void cw_get_wtp_name(struct conn *conn,struct sockaddr *addr)
{

}

int cw_set_capwap_mode(struct conn *conn,int mode)
{
	cw_dbg(DBG_INFO,"Setting CAPWAP mode to %d for %s",mode,sock_addr2str(&conn->addr));
	conn->capwap_mode_out=mode;
	return 1;
}


int cw_detect_capwap(struct conn *conn)
{
	mbag_t is = conn->incomming;

	mbag_item_t * item = mbag_get(is,CW_ITEM_WTP_SOFTWARE_VERSION);
	if (item) {
		bstrv_t s = item->data;
		uint32_t v = bstrv_get_vendor_id(s);
		
		switch(v) {
			case CW_VENDOR_ID_CISCO:
				cw_set_capwap_mode(conn,CW_MODE_CISCO);
				break;
			default:	
				cw_set_capwap_mode(conn,CW_MODE_CAPWAP);
				break;

		}
	}
	return 0;
}

int cw_in_check_cipwap_join_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from)
{


	int rc = cw_in_check_join_req(conn,a,data,len,from);


	if ( rc==0 ) {
		cw_detect_capwap(conn);

	}



	return rc;
}
