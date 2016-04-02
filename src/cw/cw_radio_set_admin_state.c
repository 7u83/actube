#include "cw.h"
#include "dbg.h"

#include "capwap_items.h"



int cw_radio_set_admin_state(mbag_t radios,int rid, int state, int cause)
{
	mbag_t radio = mbag_i_get_mbag(radios,rid,NULL);
	if (!radio) {
		cw_dbg(DBG_ELEM_ERR,"Can't set radio administrative state for radio %d - radio does't exists",rid);
		return 0;
	}
	mbag_set_byte(radio,CW_RADIOITEM_ADMIN_STATE,state);

	if (cause < 0)
		return 1;


	/* Set operational state for next config update response. */

	mbag_set_word(radio,CW_RADIOITEM_OPER_STATE, (state<<8) | 3);

	return 1;
}


