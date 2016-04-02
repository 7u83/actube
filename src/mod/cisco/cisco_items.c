#include "cw/item.h"
#include "cisco_items.h"

const char CISCO_ITEM_AP_MODE_AND_TYPE[]="cisco_ap_mode_and_type";
const char CISCO_MWAR_HASH_VALUE[]="cisco_mwar_hash_value";

struct cw_itemdef cisco_itemdefs[] = {

	{CISCO_ITEM_AP_MODE_AND_TYPE,CW_ITEM_NONE,MBAG_WORD},
	{CISCO_MWAR_HASH_VALUE,CW_ITEM_NONE,MBAG_BSTR},

	{CW_ITEM_NONE}
};
