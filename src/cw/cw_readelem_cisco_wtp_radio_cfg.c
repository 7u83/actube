

#include "capwap_cisco.h"
#include "cw_log.h"

int cw_readelem_cisco_wtp_radio_cfg(int elem_id,uint8_t *elem, int len,struct radioinfo *ri)
{
	if (elem_id != CW_CISCO_WTP_RADIO_CFG)
		return 0;

	if (len!=24){
		cw_dbg(DBG_ELEM,"LWAPP Radio Cfg element too short, %d < 21",len);
		return -1;
	}
	
	ri->rid = lw_get_byte(elem);
	ri->occupancy_limit = lw_get_word(elem+2);
	bstr_replace(&ri->bssid,bstr_create(elem+7,6));
	ri->beacon_period = lw_get_word(elem+13);

	return 1;
}	
