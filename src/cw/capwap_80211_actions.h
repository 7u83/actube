

#include "capwap80211.h"
#include "capwap_items.h"

#define CW_ACTION_IN_80211_WTP_RADIO_INFORMATION 	\
	CW_ELEM80211_WTP_RADIO_INFORMATION,		/* Element ID*/			\
	cw_in_radio_generic, 0,				/* start/end callback */	\
	MBAG_DWORD, 					/* Type of element */		\
	CW_ITEM_LOCATION_DATA,				/* ID to use store */		\
	5, 5						/* min/max length */


