
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

	cw_dbg (DBG_X,"Read State %d",state);

	int state_t = state;
	if (state == CW_RADIO_OPER_STATE_ENABLED_7)
		state_t = CW_RADIO_OPER_STATE_ENABLED;
	if (state == CW_RADIO_OPER_STATE_DISABLED_7)
		state_t = CW_RADIO_OPER_STATE_DISABLED;

	cw_dbg(DBG_X,"%d Translated state to %d",rid,state_t);

	if (rid==255){
		mbag_set_word(conn->incomming,CISCO_ITEM_WTP_OPER_STATE, (state_t<<8)|cause);
		return 1;
	}


	mbag_t radio = mbag_i_get_mbag(conn->radios,rid,NULL);
	if (!radio){
		cw_dbg(DBG_ELEM_ERR,"Bad radio ID %d for radio operational state. ID doesn't exists.",rid);
		return 0;
	}

cw_dbg(DBG_X,"Radio settng to %d",state_t << 8);

	mbag_set_word(radio,CW_RADIOITEM_OPER_STATE,(state_t<<8)|cause);
//	mbag_set_word(radio,"hitler_status",911);
//	mbag_set_word(radio,CW_RADIOITEM_OPER_STATE,912);

cw_dbg(DBG_X, "setting operstate %s",CW_RADIOITEM_OPER_STATE);

//	mbag_set_word(radio,CW_RADIOITEM_ADMIN_STATE,66);
	return 1;


}

