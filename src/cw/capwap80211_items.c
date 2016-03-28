#include "item.h"
#include "capwap80211_items.h"
#include "capwap80211_types.h"



const char CW_ITEM_80211_SUPPORTED_RATES[]="supported_rates";


struct cw_itemdef capwap80211_itemdefs[] = {

	{CW_ITEM_80211_SUPPORTED_RATES,CW_ITEM_NONE,CAPWAP80211_TYPE_RATESET},

};


