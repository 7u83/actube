/**
 * @file
 * @brief add wlan msg element implementation
 */ 

#include "capwap.h"
#include "capwap_80211.h"

#include "cwmsg.h"

#include "bstr.h"


void cwmsg_addelem_80211_add_wlan(struct cwmsg *cwmsg,struct cwwlan *wlan)
{

	uint16_t capab=htons(wlan->capab);
	uint16_t keylen=htons(wlan->key_length);
	
	cwmsg_vaddelem(cwmsg,CW_ELEM_80211_ADD_WLAN,
		&wlan->rid,1,
		&wlan->wid,1,
		&capab,2,
		&wlan->key_index,1,
		&wlan->key_status,1,
		&keylen,2,
		wlan->key,wlan->key_length,
		wlan->group_tsc,6,
		&wlan->qos,1,
		&wlan->auth_type,1,
		&wlan->mac_mode,1,
		&wlan->tunnel_mode,1,
		&wlan->suppress_ssid,1,
		bstr_data(wlan->ssid),bstr_len(wlan->ssid),
		NULL

	);
		
}

