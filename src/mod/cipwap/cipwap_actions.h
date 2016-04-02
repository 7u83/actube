

#include "capwap_cisco.h"

#define CW_ACTION_IN_CISCO_AP_MODE_AND_TYPE	 	\
	CW_CISCO_AP_MODE_AND_TYPE,			/* Element ID*/			\
	cw_in_generic, 0,				/* start/end callback */	\
	MBAG_WORD, 					/* Type of element */		\
	CW_ITEM_AP_MODE_AND_TYPE,			/* ID to use store */		\
	2, 2						/* min/max length */

#define CW_ACTION_IN_CISCO_RADIO_ADMINISTRATIVE_STATE	 	\
	CW_ELEM_RADIO_ADMINISTRATIVE_STATE,		/* Element ID*/			\
	cw_in_cisco_radio_administrative_state, 0,		/* start/end callback */	\
	MBAG_DWORD, 					/* Type of element */		\
	0,						/* ID to use store */		\
	4, 4						/* min/max length */


#define CW_ACTION_IN_CISCO_RADIO_ADMINISTRATIVE_STATE_WTP	 	\
	CW_ELEM_RADIO_ADMINISTRATIVE_STATE,		/* Element ID*/			\
	cw_in_cisco_radio_administrative_state_wtp, 0,		/* start/end callback */	\
	MBAG_DWORD, 					/* Type of element */		\
	0,						/* ID to use store */		\
	4, 4						/* min/max length */



#define CW_ACTION_IN_CISCO_RADIO_CFG\
	CW_CISCO_WTP_RADIO_CFG,			/* Element ID*/			\
	cw_in_cisco_radio_cfg, 0,			/* start/end callback */	\
	NULL, 						/* Type of element */		\
	0,						/* ID to use store */		\
	0, 1114						/* min/max length */

#define CW_ACTION_IN_CISCO_ADD_WLAN\
	CW_CISCO_ADD_WLAN,				/* Element ID*/			\
	cw_in_cisco_add_wlan, 0,			/* start/end callback */	\
	NULL, 						/* Type of element */		\
	0,						/* ID to use store */		\
	0, 1114						/* min/max length */


