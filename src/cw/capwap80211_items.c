#include "item.h"
#include "capwap80211_items.h"
#include "capwap80211_types.h"




struct cw_itemdef capwap80211_itemdefs[] = {

	{CW_ITEM_NONE}

};



const char CW_RADIOITEM80211_SUPPORTED_RATES[]="802.11 supported_rates";
const char CW_RADIOITEM80211_WTP_RADIO_INFORMATION[]="802.11 radio_info";

/* MAC Operation Items */
const char CW_RADIOITEM80211_RTS_THRESHOLD[]="rts_threshold";
const char CW_RADIOITEM80211_FRAGMENTATION_THRESHOLD[]="fragmentation_threshold";
const char CW_RADIOITEM80211_SHORT_RETRY[]="short_retry";
const char CW_RADIOITEM80211_LONG_RETRY[]="long_retry";




struct cw_itemdef capwap80211_radioitemdefs[] = {

	{CW_RADIOITEM80211_SUPPORTED_RATES,CW_ITEM_NONE,CAPWAP80211_TYPE_RATESET},
	{CW_RADIOITEM80211_WTP_RADIO_INFORMATION,CW_ITEM_NONE,MBAG_DWORD},
	{CW_ITEM_NONE}
};
