#include "item.h"
#include "capwap80211_items.h"
#include "capwap80211_types.h"



const char CW_ITEM80211_SUPPORTED_RATES[]="802.11 supported_rates";
const char CW_ITEM80211_WTP_RADIO_INFORMATION[]="802.11 radio_info";


struct cw_itemdef capwap80211_itemdefs[] = {

	{CW_ITEM80211_SUPPORTED_RATES,CW_ITEM_NONE,CAPWAP80211_TYPE_RATESET},
	{CW_ITEM80211_WTP_RADIO_INFORMATION,CW_ITEM_NONE,MBAG_DWORD},
	{CW_ITEM_NONE}

};


