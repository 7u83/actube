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


#include "cw/capwap.h"
#include "cw/capwap80211.h"
#include "cw/msgset.h"
#include "cw/ktv.h"
#include "cw/keys.h"

#include "capwap_cisco.h"
#include "mod_cisco.h"

/*#include "cw/strheap.h"
#include "cw/radio.h"
#include "cw/capwap_cisco.h"
#include "cw/capwap80211.h"
#include "cw/lwapp_cisco.h"
#include "cw/cw_80211.h"
*/

/* #include "include/capwap_actions.h" */

/*
	const struct cw_Type * type;
	const char * key;
	int len;
	int position;
*/

static cw_KTVStruct_t ap_time_sync[] = {
	{CW_TYPE_DWORD, "timestamp", 4,-1},
	{CW_TYPE_BYTE, "type", 1,-1},
	{NULL,NULL,0,0}
};

static struct cw_ElemHandler handlers[] = {
	{ 
		"WTP Descriptor (Draft 7)",	/* name */
		CAPWAP_ELEM_WTP_DESCRIPTOR,	/* Element ID */
		0,0,				/* Vendor / Proto */
		4,128,				/* min/max length */
		NULL,				/* type */
		CW_KEY_WTP_DESCRIPTOR,		/* Key */
		cisco_in_wtp_descriptor, 	/* get */
		cisco_out_wtp_descriptor	/* put */
	}
	,
	{ 
		"AC Descriptor (Cisco LWAPP)",	/* name */
		CAPWAP_ELEM_AC_DESCRIPTOR,	/* Element ID */
		0,0,				/* Vendor / Proto */
		4,128,				/* min/max length */
		NULL,				/* type */
		"ac-descriptor",		/* Key */
		cisco_in_ac_descriptor, 	/* get */
		NULL, /*cisco_out_ac_descriptor	*/	/* put */
	}
	,
	{
		"RAD Name -> CAPWAP WTP Name",	/* name */
		CW_CISCO_RAD_NAME,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,		/* Vendor / Proto */
		1,512,				/* min/max length */
		CW_TYPE_BSTR16,			/* type */
		CW_KEY_WTP_NAME,		/* Key */
		cw_in_generic,			/* get */
		cw_out_generic			/* put */
	}
	,
	{
		"Board Data Options",		/* name */
		CW_CISCO_BOARD_DATA_OPTIONS,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,		/* Vendor / Proto */
		4,4,				/* min/max length */
		CW_TYPE_DWORD,			/* type */
		"cisco/board-data-options",	/* Key */
		cw_in_generic,			/* handler */
		cw_out_generic			/* put */
	}
	,
	{
		"AP Time Sync",			/* name */
		CISCO_ELEM_AP_TIMESYNC,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,		/* Vendor / Proto */
		5,5,				/* min/max length */
		ap_time_sync,			/* type */
		"cisco/ap-timesync",		/* Key */
		cw_in_generic_struct,		/* handler */
		NULL				/* put */
	}
	,

	{
		"MWAR Type",			/* name */
		CISCO_ELEM_MWAR_TYPE,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,		/* Vendor / Proto */
		1,1,				/* min/max length */
		CW_TYPE_BYTE,			/* type */
		"cisco/mwar-type",		/* Key */
		cw_in_generic,			/* handler */
		cw_out_generic			/* put */
	}
	,

	{0,0,0,0,0,0,0,0}

};


static int discovery_request_states[] = {CAPWAP_STATE_DISCOVERY,0};
static struct cw_ElemDef discovery_request_elements[] ={
/*	{0,0,			CAPWAP_ELEM_WTP_DESCRIPTOR,	1, 0},*/
	{0,0,			CAPWAP_ELEM_WTP_BOARD_DATA,	0, 0},
	{0,CW_VENDOR_ID_CISCO,	CW_CISCO_RAD_NAME,		1, 0},
	{0,CW_VENDOR_ID_CISCO,	CW_CISCO_BOARD_DATA_OPTIONS,	0, 0},
	{0,0,0,00}
	
};

static int discovery_response_states[] = {CAPWAP_STATE_DISCOVERY,0};
static struct cw_ElemDef discovery_response_elements[] ={
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_TIMESYNC,			1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_MWAR_TYPE,			0, 0},
	{0,0,			CAPWAP80211_ELEM_WTP_RADIO_INFORMATION,	0, 0},
	{0,0,0,00}
	
};


static struct cw_MsgDef messages[] = {
	{
		NULL,				/* name */
		CAPWAP_MSG_DISCOVERY_REQUEST,	/* type */
		CW_RECEIVER_AC,			
		discovery_request_states,
		discovery_request_elements
	},
	{
		NULL,				/* name */
		CAPWAP_MSG_DISCOVERY_RESPONSE,	/* type */
		CW_RECEIVER_WTP,
		discovery_response_states,
		discovery_response_elements
	},
	{0,0,0,0}

};






struct cw_MsgSet * cisco_register_msg_set(struct cw_MsgSet * set, int mode){
        if (mode != CW_MOD_MODE_CAPWAP)
                return NULL;
        cw_msgset_add(set,messages, handlers);
        return set;
}

