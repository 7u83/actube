#include "cw/item.h"
#include "cisco_items.h"

const char CISCO_ITEM_AP_MODE_AND_TYPE[]="cisco_ap_mode_and_type";

struct cw_itemdef cisco_itemdefs[] = {

	{CISCO_ITEM_AP_MODE_AND_TYPE,CW_ITEM_NONE,MBAG_WORD},

	{CW_ITEM_NONE}
};



const char CISCO_RADIOITEM80211_CFG_TYPE[]="cisco_radio_cfg_type";
const char CISCO_RADIOITEM80211_COUNTRY_STR1[]="cisco_country_str1";
const char CISCO_RADIOITEM80211_COUNTRY_STR2[]="cisco_country_str2";

struct cw_itemdef cisco_radioitemdefs[] = {

	{CISCO_RADIOITEM80211_CFG_TYPE,CW_ITEM_NONE,MBAG_BYTE},
	{CISCO_RADIOITEM80211_COUNTRY_STR1,CW_ITEM_NONE,MBAG_BSTR16},
	{CISCO_RADIOITEM80211_COUNTRY_STR2,CW_ITEM_NONE,MBAG_BSTR16},

	{CW_ITEM_NONE}
};
