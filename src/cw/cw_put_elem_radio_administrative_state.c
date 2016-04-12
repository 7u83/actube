#include "cw.h"
#include "capwap_items.h"

int xcw_put_elem_radio_administrative_state(uint8_t *dst,int radio_id,mbag_t radio)
{
	cw_put_byte(dst+4,radio_id);
	cw_put_byte(dst+5,mbag_get_byte(radio,CW_RADIOITEM_ADMIN_STATE,CW_RADIO_ADMIN_STATE_DISABLED));
	return 2 + cw_put_elem_hdr(dst,CW_ELEM_RADIO_ADMINISTRATIVE_STATE,2);
}


