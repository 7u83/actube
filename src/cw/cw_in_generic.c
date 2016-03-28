
#include "action.h"
#include "dbg.h"
#include "log.h"
#include "mbag.h"
#include "cw.h"


int static check_len(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		     struct sockaddr *from)
{
	if (len < a->min_len) {
		cw_dbg(DBG_ELEM_ERR,
		       "%d (%s) message element too short, len=%d, min len=%d",
		       a->elem_id, cw_strelemp(conn->actions, a->elem_id), len,
		       a->min_len);
		return 0;
	}
	if (len > a->max_len) {
		cw_dbg(DBG_ELEM_ERR,
		       "%d (%s) message element too big, len=%d, max len=%d", a->elem_id,
		       cw_strelemp(conn->actions, a->elem_id), len, a->max_len);
		return 0;
	}

	return 1;
}


int static do_save(mbag_t itemstore, struct conn *conn, struct cw_action_in *a,
		   uint8_t * data, int len, struct sockaddr *from)
{

	if (a->itemtype == MBAG_BYTE) {
		mbag_set_byte(itemstore, a->item_id, *data);
		return 1;
	}
	if (a->itemtype == MBAG_WORD) {
		mbag_set_word(itemstore, a->item_id, cw_get_word(data));
		return 1;
	}
	if (a->itemtype == MBAG_DWORD) {
		mbag_set_dword(itemstore, a->item_id, cw_get_dword(data));
		return 1;
	}
	if (a->itemtype == MBAG_STR) {
		mbag_set_strn(itemstore, a->item_id, (char *) data, len);
		return 1;
	}
	if (a->itemtype == MBAG_BSTR) {
		mbag_set_bstrn(itemstore, a->item_id, data, len);
		return 1;
	}

	if (a->itemtype == MBAG_BSTR16) {
		mbag_set_bstr16n(itemstore, a->item_id, data, len);
		return 1;
	}

/*	if (a->itemtype == MBAG_DATA) {
		mbag_set_data(itemstore, a->item_id, data, len);
		return 1;
	}
*/
	if (a->itemtype == MBAG_VENDORSTR) {
		mbag_set_bstrv(itemstore, a->item_id,
				   cw_get_dword(data), data + 4, len - 4);
		return 1;
	}

	cw_log(LOG_ERR,
	       "Can't handle item type %d in definition for incomming msg %d (%s) - %d, cw_in_generic.",
	       a->itemtype, a->msg_id, cw_strmsg(a->msg_id), a->elem_id);
	return 0;


}

int cw_in_generic(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{

	if (!check_len(conn, a, data, len, from))
		return 0;


	mbag_t itemstore;
///	if (!a->target)
		itemstore = conn->incomming;
//	else
//		itemstore = a->target(conn, a);


	return do_save(itemstore, conn, a, data, len, from);



}
/*
int cw_in_radio_generic(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			int len, struct sockaddr *from)
{
//	if (!check_len(conn, a, data, len, from))
//		return 0;

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
	}



	return 1;
}

*/
