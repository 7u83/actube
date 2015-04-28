


#include "radio.h"
#include "dbg.h"


/** 
 * Update all radio operational states.
 * @param radios definitiion of radios
 * @param cause cause to set
 */ 
int cw_radio_update_oper_states(mbag_t radios,int cause)
{
	MAVLITER_DEFINE(it,radios);
	mavliter_foreach(&it){
		mbag_item_t * i = mavliter_get(&it);
		
		/* Get admin state of the radio, set disabled (2) if admin state is
		   not defined */
		int admin_state = mbag_get_byte(i->data,CW_RADIO_ADMIN_STATE,2);
		mbag_set_word(i->data,CW_RADIO_OPER_STATE,(admin_state<<8) | cause );

	}
	return 1;
}





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


