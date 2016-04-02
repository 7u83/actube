
#include "action.h"
#include "dbg.h"
#include "log.h"
#include "mbag.h"
#include "cw.h"

#include "item.h"





int cw_in_radio_generic(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			int len, struct sockaddr *from)
{

	const cw_itemdef_t * idef = cw_itemdef_get(conn->actions->radioitems,a->item_id,CW_ITEM_NONE);
	if (!idef){
		cw_log(LOG_ERR,"No definition found for %s",a->item_id);
		return 0;
	}


	int rid = cw_get_byte(data);
	mbag_t radio = mbag_i_get_mbag(conn->radios, rid, NULL);
	if (!radio) {
		if (a->vendor_id != 0
		    || ( (a->vendor_id == 0) && (a->msg_id != CW_MSG_DISCOVERY_REQUEST
			&& a->msg_id != CW_MSG_JOIN_REQUEST) )) {
			cw_dbg(DBG_ELEM_ERR, "Radio not found %d", rid);
			return 0;
		}
		mbag_i_set_mbag(conn->radios,rid,mbag_create());
		radio = mbag_i_get_mbag(conn->radios, rid, NULL);
	}

	int rc = mbag_set_from_buf(radio,idef->type,a->item_id,data+1,len-1);
	if (!rc){
		cw_log(LOG_ERR,
		       "Can't handle item type %s in definition for incomming msg %d (%s) - %d, cw_in_radio_generic.",
		       idef->type->name, a->msg_id, cw_strmsg(a->msg_id), a->elem_id);
	}

	return rc;


}


