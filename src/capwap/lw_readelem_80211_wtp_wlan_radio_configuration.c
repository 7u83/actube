

#include "lwapp.h"

int lw_readelem_80211_wtp_wlan_radio_configuration(int elem_id,uint8_t *elem, int len,struct radioinfo *ri)
{
	if (elem_id != LW_ELEM_80211_WLAN_RADIO_CONFIGURATION)
		return 1;

	if (len<21){
		cw_dbg(DBG_ERR,"LWAPP Radio Cfg element too short, %d < 21",len);
		return -1;
	}
	
	ri->rid = lw_get_byte(elem);
	ri->occupancy_limit = lw_get_word(elem+2);
	
	bstr_replace(ri->bssid,bstr_create(elem+7,6);
	

}
