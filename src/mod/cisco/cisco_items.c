#include "cw/item.h"
#include "cisco_items.h"

const char CISCO_ITEM_AP_MODE_AND_TYPE[]="cisco_ap_mode_and_type";
const char CISCO_ITEM_AP_LED_STATE_CONFIG[]="cisco_ap_led_state";
const char CISCO_ITEM_AP_LED_FLASH_CONFIG[]="cisco_ap_led_flash";

struct cw_itemdef cisco_itemdefs[] = {

	{CISCO_ITEM_AP_MODE_AND_TYPE,CW_ITEM_NONE,MBAG_WORD},
	{CISCO_ITEM_AP_LED_STATE_CONFIG,CW_ITEM_NONE,MBAG_WORD},
	{CISCO_ITEM_AP_LED_FLASH_CONFIG,CW_ITEM_NONE,MBAG_BIN},

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
