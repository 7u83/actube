
#include "mbag.h"
#include "item.h"

#include "cipwap_items.h"



const char CIPWAP_ITEM_SSH_ENABLE[]="ssh_enable";
const char CIPWAP_ITEM_TELNET_ENABLE[]="telnet_enable";

struct cw_itemdef cipwap_itemdefs[] = {

	{CIPWAP_ITEM_TELNET_ENABLE,CW_ITEM_NONE,MBAG_BYTE},
	{CIPWAP_ITEM_SSH_ENABLE,CW_ITEM_NONE,MBAG_BYTE},
	{CW_ITEM_NONE}

};

