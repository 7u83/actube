/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __CAPWAP_ACTIONS_H
#define __CAPWAP_ACTIONS_H


#include "capwap.h"
#include "capwap_items.h"


#define CW_ACTION_IN_LOCATION_DATA 	\
	CW_ELEM_LOCATION_DATA, 		/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_STR, 		/* Type of element */		\
	CW_ITEM_LOCATION_DATA,		/* ID to use store */		\
	1, 1024				/* min/max length */


#define CW_ACTION_IN_WTP_NAME	 	\
	CW_ELEM_WTP_NAME, 		/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_STR, 		/* Type of element */		\
	CW_ITEM_WTP_NAME,		/* ID to use store */		\
	1, 1024				/* min/max length */

#define CW_ACTION_IN_SESSION_ID	 	\
	CW_ELEM_SESSION_ID, 		/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_BSTR, 		/* Type of element */		\
	CW_ITEM_SESSION_ID,		/* ID to use store */		\
	16, 16				/* min/max length */

#define CW_ACTION_IN_IMAGE_IDENTIFIER	 	\
	CW_ELEM_IMAGE_IDENTIFIER,	/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_VENDORSTR, 		/* Type of element */		\
	CW_ITEM_IMAGE_IDENTIFIER,	/* ID to use store */		\
	5, 4096				/* min/max length */

#define CW_ACTION_IN_AC_NAME	 	\
	CW_ELEM_AC_NAME, 		/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_STR, 		/* Type of element */		\
	CW_ITEM_AC_NAME,		/* ID to use store */		\
	1, 512				/* min/max length */

#define CW_ACTION_IN_STATISTICS_TIMER	 	\
	CW_ELEM_STATISTICS_TIMER,	/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_WORD,	 		/* Type of element */		\
	CW_ITEM_STATISTICS_TIMER,	/* ID to use store */		\
	2, 2				/* min/max length */

#define CW_ACTION_IN_WTP_REBOOT_STATISTICS	 	\
	CW_ELEM_WTP_REBOOT_STATISTICS,	/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_DATA, 		/* Type of element */		\
	CW_ITEM_WTP_REBOOT_STATISTICS,	/* ID to use store */		\
	15, 15				/* min/max length */


#define CW_ACTION_IN_WTP_FRAME_TUNNEL_MODE	 	\
	CW_ELEM_WTP_FRAME_TUNNEL_MODE,	/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_BYTE, 		/* Type of element */		\
	CW_ITEM_WTP_FRAME_TUNNEL_MODE,	/* ID to use store */		\
	1, 1				/* min/max length */


#define CW_ACTION_IN_WTP_MAC_TYPE	 	\
	CW_ELEM_WTP_MAC_TYPE,		/* Element ID*/			\
	cw_in_generic, 0,		/* start/end callback */	\
	MBAG_BYTE, 		/* Type of element */		\
	CW_ITEM_WTP_MAC_TYPE,		/* ID to use store */		\
	1, 1				/* min/max length */

#define CW_ACTION_IN_VENDOR_SPECIFIC_PAYLOAD	\
	CW_ELEM_VENDOR_SPECIFIC_PAYLOAD,	/* Element ID */	\
	cw_in_vendor_specific_payload,0,	/* start/end callback*/	\
	0,								\
	0,								\
	0,0								
	
#define CW_ACTION_IN_WTP_BOARD_DATA	\
	CW_ELEM_WTP_BOARD_DATA,			/* Element ID */		\
	cw_in_wtp_board_data, 0,		/* start/end callback */	\
	0,									\
	CW_ITEM_WTP_BOARD_DATA,							\
	0,0									

#define CW_ACTION_IN_WTP_DESCRIPTOR	\
	CW_ELEM_WTP_DESCRIPTOR,			/* Element ID */		\
	cw_in_wtp_descriptor, 0,		/* start/end callback */	\
	0,									\
	CW_ITEM_WTP_DESCRIPTOR,							\
	0,0								
	

#define CW_ACTION_IN_CAPWAP_CONTROL_IPV4_ADDRESS	 	\
	CW_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS,		/* Element ID*/			\
	cw_in_capwap_control_ip_address, 0,		/* start/end callback */	\
	MBAG_AVLTREE, 				/* Type of element */		\
	CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST,		/* ID to use store */		\
	6, 6						/* min/max length */

#define CW_ACTION_IN_CAPWAP_CONTROL_IPV6_ADDRESS	 	\
	CW_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS,		/* Element ID*/			\
	cw_in_capwap_control_ip_address, 0,		/* start/end callback */	\
	MBAG_AVLTREE, 				/* Type of element */		\
	CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST,		/* ID to use store */		\
	18,18						/* min/max length */



#define CW_ACTION_IN_AC_DESCRIPTOR	 	\
	CW_ELEM_AC_DESCRIPTOR,				/* Element ID*/			\
	cw_in_ac_descriptor, 0,				/* start/end callback */	\
	MBAG_DATA, 				/* Type of element */		\
	CW_ITEM_AC_DESCRIPTOR,				/* ID to use store */		\
	12, 8192						/* min/max length */


#define CW_ACTION_IN_RESULT_CODE	 	\
	CW_ELEM_RESULT_CODE,				/* Element ID*/			\
	cw_in_generic, 0,				/* start/end callback */	\
	MBAG_DWORD, 				/* Type of element */		\
	CW_ITEM_RESULT_CODE,				/* ID to use store */		\
	4, 4						/* min/max length */

#define CW_ACTION_IN_CAPWAP_TIMERS	 	\
	CW_ELEM_CAPWAP_TIMERS,				/* Element ID*/			\
	cw_in_generic, 0,				/* start/end callback */	\
	MBAG_WORD, 				/* Type of element */		\
	CW_ITEM_CAPWAP_TIMERS,				/* ID to use store */		\
	2, 2						/* min/max length */

#define CW_ACTION_IN_IDLE_TIMEOUT	 	\
	CW_ELEM_IDLE_TIMEOUT,				/* Element ID*/			\
	cw_in_generic, 0,				/* start/end callback */	\
	MBAG_DWORD, 					/* Type of element */		\
	CW_ITEM_IDLE_TIMEOUT,				/* ID to use store */		\
	4, 4						/* min/max length */


#define CW_ACTION_IN_AC_NAME_WITH_PRIORITY	 	\
	CW_ELEM_IDLE_TIMEOUT,				/* Element ID*/			\
	cw_in_generic, 0,				/* start/end callback */	\
	MBAG_DWORD, 					/* Type of element */		\
	CW_ITEM_IDLE_TIMEOUT,				/* ID to use store */		\
	4, 4						/* min/max length */



#define CW_ACTION_IN_RADIO_ADMINISTRATIVE_STATE	 	\
	CW_ELEM_RADIO_ADMINISTRATIVE_STATE,		/* Element ID*/			\
	cw_in_radio_administrative_state, 0,		/* start/end callback */	\
	MBAG_DWORD, 					/* Type of element */		\
	0,						/* ID to use store */		\
	4, 4						/* min/max length */

#define CW_ACTION_IN_RADIO_ADMINISTRATIVE_STATE_WTP 	\
	CW_ELEM_RADIO_ADMINISTRATIVE_STATE,		/* Element ID*/			\
	cw_in_radio_administrative_state_wtp, 0,		/* start/end callback */	\
	MBAG_DWORD, 					/* Type of element */		\
	0,						/* ID to use store */		\
	4, 4						/* min/max length */




#define CW_ACTION_IN_MAXIMUM_MESSAGE_LENGTH	 	\
	CW_ELEM_MAXIMUM_MESSAGE_LENGTH,				/* Element ID*/			\
	cw_in_generic, 0,				/* start/end callback */	\
	MBAG_WORD, 				/* Type of element */		\
	CW_ITEM_MAXIMUM_MESSAGE_LENGTH,			/* ID to use store */		\
	2, 2						/* min/max length */



#endif
