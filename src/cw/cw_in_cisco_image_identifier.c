
#include "action.h"
#include "dbg.h"
#include "mbag.h"
#include "capwap.h"
#include "capwap_cisco.h"


int cw_in_cisco_image_identifier(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len,struct sockaddr *from)
{

	if (len<a->min_len) {
		cw_dbg(DBG_ELEM_ERR,"Message element too short, %d < %d", len,a->min_len);
		return 0;
	}

	uint32_t vendor_id = cw_get_dword(data);
	int dstart;

	switch (vendor_id) {
		case CW_VENDOR_ID_ZYXEL:
		case CW_VENDOR_ID_CISCO:
		case CW_VENDOR_ID_FSF:
		case 0:
			dstart=4;
			len-=4;
			break;
		default:
			vendor_id=CW_VENDOR_ID_CISCO;
			dstart=0;
	}

//	mbag_set(conn->remote,a->item_id,a->itemtype,data+dstart,len);
	mbag_set_bstrv(conn->incomming,a->item_id,vendor_id,data+dstart,len);
	return 1;
}

