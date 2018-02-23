
#include "cw/action.h"
#include "cw/dbg.h"
#include "cw/log.h"
#include "cw/mbag.h"
#include "cw/capwap.h"
#include "cw/capwap_actions.h"
#include "cw/cw.h"

#include "cisco_items.h"

int cisco_in_radio_operational_state(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			int len, struct sockaddr *from)
{
	int rid = cw_get_byte(data);
	int state = cw_get_byte(data+1); 
	int cause = cw_get_byte(data+2);


	int state_t = state;
	if (state == CW_RADIO_OPER_STATE_ENABLED_7)
		state_t = CW_RADIO_OPER_STATE_ENABLED;
	if (state == CW_RADIO_OPER_STATE_DISABLED_7)
		state_t = CW_RADIO_OPER_STATE_DISABLED;


	if (rid==255){
		mbag_set_word(conn->incomming,CISCO_ITEM_WTP_OPER_STATE, (state_t<<8)|cause);
		return 1;
	}


	mbag_t radio = mbag_i_get_mbag(conn->radios,rid,NULL);
	if (!radio){
		cw_dbg(DBG_ELEM_ERR,"Bad radio ID %d for radio operational state. ID doesn't exists.",rid);
		return 0;
	}

	char * strstate = state_t == CW_RADIO_OPER_STATE_ENABLED ? "enabled":"disabled";
	cw_dbg(DBG_ELEM_DETAIL,"\tDetail: Radio ID=%d, State=%s, Cause: %d",rid,strstate, cause);

	mbag_set_word(radio,CW_RADIOITEM_OPER_STATE,(state_t<<8)|cause);

	return 1;


}

