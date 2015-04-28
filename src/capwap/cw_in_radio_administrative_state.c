
#include "action.h"
#include "dbg.h"
#include "log.h"
#include "mbag.h"
#include "capwap.h"
#include "radio.h"


int cw_radio_set_admin_state(mbag_t radios,int rid, int state, int cause)
{
	mbag_t radio = mbag_get_mbag(radios,rid,NULL);
	if (!radio) {
		cw_dbg(DBG_ELEM_ERR,"Can't set radio administrative state for radio %d - radio does't exists",rid);
		return 0;
	}
	mbag_set_byte(radio,CW_RADIO_ADMIN_STATE,state);

	if (cause < 0)
		return 1;


	/* Set operational state for next config update response. */

	mbag_set_word(radio,CW_RADIO_OPER_STATE, (state<<8) | 3);

	return 1;
}


int cw_in_radio_administrative_state(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{
	int rid = cw_get_byte(data);
	int state = cw_get_byte(data+1);
	return cw_radio_set_admin_state(conn->radios,rid,state,-1);
}



