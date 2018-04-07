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
#include "cw/proto.h"
#include "lwapp_cisco.h"

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

static cw_KTVStruct_t mwar_addr[] = {
	{CW_TYPE_BYTE, "mwar-type", 1,-1},
	{CW_TYPE_IPADDRESS, "address", 4,-1},
	{CW_TYPE_WORD, "unknown", 2,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_lw_path_mtu[] = {
	{CW_TYPE_WORD, "max", 2,-1},
	{CW_TYPE_WORD, "len", 2,-1},
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
		cisco_out_ac_descriptor		/* put */
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
		cisco_out_ap_timesync		/* put */
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
	{
		"80211 WTP Radio Information - Cisco",			/* name */
		CAPWAP80211_ELEM_WTP_RADIO_INFORMATION,		/* Element ID */
		0, 0,						/* Vendor / Proto */
		0, 0,						/* min/max length */
		CW_TYPE_DWORD,					/* type */
		"wtp-radio-information",			/* Key */
		cw_in_radio_generic,				/* get */
		cisco_out_radio_generic				/* put */
	}
	,
	{ 
		"Session ID (Cisco min len = 4)",	/* name */
		CAPWAP_ELEM_SESSION_ID,			/* Element ID */
		0,0,					/* Vendor / Proto */
		4,CAPWAP_SESSION_ID_LEN,		/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"session-id",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"AP Group Name",			/* name */
		CISCO_ELEM_AP_GROUP_NAME,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,512,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/ap-group-name",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"MWAR Addr",				/* name */
		CISCO_ELEM_MWAR_ADDR,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		7,7,					/* min/max length */
		mwar_addr,				/* type */
		"cisco/mwar-addr",			/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	}
	,
	{ 
		"WTP IPv4 Address (Draft 7)",		/* name */
		CAPWAP_ELEM_WTP_IPV4_IP_ADDRESS,	/* Element ID */
		0,0,					/* Vendor / Proto */
		4,4,					/* min/max length */
		CW_TYPE_IPADDRESS,			/* type */
		"capwap-local-ip-address",		/* Key */
		cw_in_generic,				/* get */
		cisco_out_capwap_local_ip_address	/* put */
	}
	,

	{ 
		"WTP IPv6 Address (Draft 7)",		/* name */
		CAPWAP_ELEM_WTP_IPV6_IP_ADDRESS,	/* Element ID */
		0,0,					/* Vendor / Proto */
		16,16,					/* min/max length */
		CW_TYPE_IPADDRESS,			/* type */
		"capwap-local-ip-address",		/* Key */
		cw_in_generic,				/* get */
		cisco_out_capwap_local_ip_address	/* put */
	}
	,
	{ 
		"SPAM Vendor Specific",			/* name */
		CISCO_ELEM_SPAM_VENDOR_SPECIFIC,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		0,0,					/* min/max length */
		NULL,					/* type */
		"cisco_spam_vendor_specific",		/* Key */
		cisco_in_spam_vendor_specific,		/* get */
		NULL		/* put */
	}
	,
	{ 
		"Path MTU",				/* name */
		CISCO_LWELEM_PATH_MTU,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		0,0,					/* min/max length */
		cisco_lw_path_mtu,			/* type */
		"cisco/lw_path_mtu",			/* Key */
		cw_in_generic_struct,			/* get */
		cisco_out_lw_path_mtu			/* put */
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

static int join_request_states[] = {CAPWAP_STATE_JOIN,0};
static struct cw_ElemDef join_request_elements[] ={
	{0,CW_VENDOR_ID_CISCO,	CW_CISCO_BOARD_DATA_OPTIONS,	1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_GROUP_NAME,	1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_MWAR_ADDR,		1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,1, CW_IGNORE},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PATH_MTU,	0, 0},

	
	{0,0,			CAPWAP_ELEM_WTP_IPV4_IP_ADDRESS,1,0},
	{0,0,			CAPWAP_ELEM_WTP_IPV6_IP_ADDRESS,1,0},

	{0,0,CAPWAP_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS,		0, CW_DELETE},
	{0,0,CAPWAP_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS,		0, CW_DELETE},
	{0,0,			CAPWAP_ELEM_ECN_SUPPORT,	0, CW_DELETE},


	{0,0,0,00}
	
};



static struct cw_MsgDef messages[] = {
	{
		NULL,				/* name */
		CAPWAP_MSG_DISCOVERY_REQUEST,	/* type */
		CW_ROLE_AC,			
		discovery_request_states,
		discovery_request_elements
	},
	{
		NULL,				/* name */
		CAPWAP_MSG_DISCOVERY_RESPONSE,	/* type */
		CW_ROLE_WTP,
		discovery_response_states,
		discovery_response_elements
	},
	{
		NULL,				/* name */
		CAPWAP_MSG_JOIN_REQUEST,	/* type */
		CW_ROLE_AC,
		join_request_states,
		join_request_elements
	},
	{0,0,0,0}

};






struct cw_MsgSet * cisco_register_msg_set(struct cw_MsgSet * set, int mode){
        if (mode != CW_MOD_MODE_CAPWAP)
                return NULL;
        cw_msgset_add(set,messages, handlers);
        return set;
}

