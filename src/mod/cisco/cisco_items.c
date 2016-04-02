#include "cw/item.h"
#include "cisco_items.h"

const char CISCO_ITEM_AP_MODE_AND_TYPE[]="cisco_ap_mode_and_type";

struct cw_itemdef cisco_itemdefs[] = {

	{CISCO_ITEM_AP_MODE_AND_TYPE,CW_ITEM_NONE,MBAG_WORD},

	{CW_ITEM_NONE}
};
