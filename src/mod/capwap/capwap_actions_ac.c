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


#include "cw/cw.h"
#include "cw/ktv.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/msgset.h"
#include "cw/keys.h"

#include "mod_capwap.h"


static int postprocess_join_request(struct conn *conn);


static cw_KTVStruct_t wtp_reboot_statistics[] = {
	{CW_TYPE_WORD, "reboot-count", 2,-1},
	{CW_TYPE_WORD, "ac-initiated-count", 2,-1},
	{CW_TYPE_WORD, "link-failure-count", 2,-1},
	{CW_TYPE_WORD, "sw-failure-count", 2,-1},
	{CW_TYPE_WORD, "hw-failure-count", 2,-1},
	{CW_TYPE_WORD, "other-failure-count", 2,-1},
	{CW_TYPE_WORD, "unknown-failure-count", 2,-1},
	{CW_TYPE_BYTE, "last-failure-type", 1,-1},	
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t capwap_timers[] = {
	{CW_TYPE_BYTE, "max-discovery-interval", 1,-1},
	{CW_TYPE_BYTE, "echo-interval", 1,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t radio_operational_state[] = {
	{CW_TYPE_BYTE, "state", 1,-1},
	{CW_TYPE_BYTE, "cause", 1,-1},
	{NULL,NULL,0,0}
};


static cw_KTVValRange_t radio_admin_state_valguard[]={
	{0,0,"reserved"},
	{1,1,"enabled"},
	{2,2,"disabled"},
	{0,0,NULL}
};

static cw_KTVStruct_t radio_admin_state[] = {
	{CW_TYPE_BYTE, NULL, 1,-1, radio_admin_state_valguard},
	{NULL,NULL,0,0}
};

static struct cw_ElemHandler handlers[] = {

	{ 
		"Discovery Type",			/* name */
		CAPWAP_ELEM_DISCOVERY_TYPE,		/* Element ID */
		0,0,					/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		CW_KEY_DISCOVERY_TYPE,			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,
	{ 
		"WTP Mac Type",				/* name */
		CAPWAP_ELEM_WTP_MAC_TYPE,		/* Element ID */
		0,0,					/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		CW_KEY_WTP_MAC_TYPE,			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,
	{ 
		"WTP Board Data",			/* name */
		CAPWAP_ELEM_WTP_BOARD_DATA,		/* Element ID */
		0,0,					/* Vendor / Proto */
		14,1024,				/* min/max length */
		NULL,					/* type */
		CW_KEY_WTP_BOARD_DATA,			/* Key */
		capwap_in_wtp_board_data,		/* get */
		capwap_out_wtp_board_data		/* put */
	}
	,
	{ 
		"WTP Descriptor",			/* name */
		CAPWAP_ELEM_WTP_DESCRIPTOR,		/* Element ID */
		0,0,					/* Vendor / Proto */
		33,1024,				/* min/max length */
		NULL,					/* type */
		CW_KEY_WTP_DESCRIPTOR,			/* Key */
		capwap_in_wtp_descriptor,		/* get */
		capwap_out_wtp_descriptor,		/* put */
	}
	,
	{ 
		"WTP Frame Tunnel Mode",		/* name */
		CAPWAP_ELEM_WTP_FRAME_TUNNEL_MODE,	/* Element ID */
		0,0,					/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		CW_KEY_WTP_FRAME_TUNNEL_MODE,		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,
	{ 
		"Vendor Specific Payload",		/* name */
		CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	/* Element ID */
		0,0,					/* Vendor / Proto */
		0,0,					/* min/max length */
		NULL,					/* type */
		"vendor_specific_payload",		/* Key */
		capwap_in_vendor_specific_payload,	/* get */
		NULL					/* put */
	}
	,
	{ 
		"MTU Discovery Padding",		/* name */
		CAPWAP_ELEM_MTU_DISCOVERY_PADDING,	/* Element ID */
		0,0,					/* Vendor / Proto */
		0,0,					/* min/max length */
		NULL,					/* type */
		"mtu_discovery_padding",		/* Key */
		capwap_in_mtu_discovery_padding,	/* get */
		NULL					/* put */
	}
	,

	{ 
		"AC Descriptor",			/* name */
		CAPWAP_ELEM_AC_DESCRIPTOR,		/* Element ID */
		0,0,					/* Vendor / Proto */
		0,0,					/* min/max length */
		NULL,					/* type */
		CW_KEY_AC_DESCRIPTOR,			/* Key */
		capwap_in_ac_descriptor,		/* get */
		capwap_out_ac_descriptor		/* put */
	}
	,

	{ 
		"AC Name",				/* name */
		CAPWAP_ELEM_AC_NAME,			/* Element ID */
		0,0,					/* Vendor / Proto */
		1,CAPWAP_MAX_AC_NAME_LEN,		/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"ac-name",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"CAPWAP Control IPv4 Address",		/* name */
		CAPWAP_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS,	/* Element ID */
		0,0,					/* Vendor / Proto */
		6,6,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"capwap-control-ip-address",		/* Key */
		capwap_in_capwap_control_ip_address,	/* get */
		capwap_out_capwap_control_ip_address	/* put */
	},
	
	{ 
		"CAPWAP Control IPv6 Address",		/* name */
		CAPWAP_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS,	/* Element ID */
		0,0,					/* Vendor / Proto */
		18,18,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"capwap-control-ip-address",		/* Key */
		capwap_in_capwap_control_ip_address,	/* get */
		capwap_out_capwap_control_ip_address	/* put */
	}
	
	,

	{ 
		"Location Data",			/* name */
		CAPWAP_ELEM_LOCATION_DATA,		/* Element ID */
		0,0,					/* Vendor / Proto */
		1,CAPWAP_MAX_LOCATION_DATA_LEN,		/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"location-data",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"WTP Name",				/* name */
		CAPWAP_ELEM_WTP_NAME,			/* Element ID */
		0,0,					/* Vendor / Proto */
		1,CAPWAP_MAX_WTP_NAME_LEN,		/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"wtp-name",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,
	{ 
		"Session ID",				/* name */
		CAPWAP_ELEM_SESSION_ID,			/* Element ID */
		0,0,					/* Vendor / Proto */
		CAPWAP_SESSION_ID_LEN,CAPWAP_SESSION_ID_LEN,	/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"session-id",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"Result Code",				/* name */
		CAPWAP_ELEM_RESULT_CODE,		/* Element ID */
		0,0,					/* Vendor / Proto */
		4,4,					/* min/max length */
		CW_TYPE_DWORD,				/* type */
		"result-code",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"ECN Support",				/* name */
		CAPWAP_ELEM_ECN_SUPPORT,		/* Element ID */
		0,0,					/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		"ecn-support",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"Maximum Message Length",		/* name */
		CAPWAP_ELEM_MAXIMUM_MESSAGE_LENGTH,	/* Element ID */
		0,0,					/* Vendor / Proto */
		2,2,					/* min/max length */
		CW_TYPE_WORD,				/* type */
		"maximum-message-length",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"CAPWAP Local IPv4 Address",		/* name */
		CAPWAP_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS,	/* Element ID */
		0,0,					/* Vendor / Proto */
		4,4,					/* min/max length */
		CW_TYPE_IPADDRESS,			/* type */
		"capwap-local-ip-address",		/* Key */
		cw_in_generic,				/* get */
		capwap_out_capwap_local_ip_address	/* put */
	}
	,
	{ 
		"CAPWAP Local IPv6 Address",		/* name */
		CAPWAP_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS,	/* Element ID */
		0,0,					/* Vendor / Proto */
		16,16,					/* min/max length */
		CW_TYPE_IPADDRESS,			/* type */
		"capwap-local-ip-address",		/* Key */
		cw_in_generic,				/* get */
		capwap_out_capwap_local_ip_address	/* put */
	}

	,
	
	{ 
		"Statitsics Timer",			/* name */
		CAPWAP_ELEM_STATISTICS_TIMER,		/* Element ID */
		0,0,					/* Vendor / Proto */
		2,2,					/* min/max length */
		CW_TYPE_WORD,				/* type */
		"statistics-timer",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{
		"WTP Reboot Statistics",		/* name */
		CAPWAP_ELEM_WTP_REBOOT_STATISTICS,	/* Element ID */
		0,0,					/* Vendor / Proto */
		15,15,					/* min/max length */
		wtp_reboot_statistics,			/* type */
		"wtp-reboot-statistics",		/* Key */
		cw_in_generic_struct,			/* handler */
		cw_out_generic_struct			/* put */
	}
	,

	{
		"Radio Administrative State",			/* name */
		CAPWAP_ELEM_RADIO_ADMINISTRATIVE_STATE,		/* Element ID */
		0, 0,						/* Vendor / Proto */
		2, 2,						/* min/max length */
		radio_admin_state,					/* type */
		"admin-state",					/* Key */
		cw_in_radio_generic_struct,				/* get */
		cw_out_radio_generic_struct				/* put */
	}
	,

	{
		"CAPWAP Timers",			/* name */
		CAPWAP_ELEM_CAPWAP_TIMERS,		/* Element ID */
		0, 0,						/* Vendor / Proto */
		2, 2,						/* min/max length */
		capwap_timers,					/* type */
		"capwap-timers",					/* Key */
		cw_in_generic_struct,				/* get */
		cw_out_generic_struct				/* put */
	}
	,
	{
		"Decryption Error Report Period",		/* name */
		CAPWAP_ELEM_DECRYPTION_ERROR_REPORT_PERIOD,	/* Element ID */
		0, 0,						/* Vendor / Proto */
		3, 3,						/* min/max length */
		CW_TYPE_WORD,					/* type */
		"decryption-error-report-period",		/* Key */
		cw_in_radio_generic,				/* get */
		cw_out_radio_generic				/* put */
	}
	,

	{
		"Idle Timeout",					/* name */
		CAPWAP_ELEM_IDLE_TIMEOUT,			/* Element ID */
		0, 0,						/* Vendor / Proto */
		4, 4,						/* min/max length */
		CW_TYPE_DWORD,					/* type */
		"idle-timeout",					/* Key */
		cw_in_generic,					/* get */
		cw_out_generic					/* put */
	}
	,

	{
		"WTP Fallback",					/* name */
		CAPWAP_ELEM_WTP_FALLBACK,			/* Element ID */
		0, 0,						/* Vendor / Proto */
		1, 1,						/* min/max length */
		CW_TYPE_BYTE,					/* type */
		"wtp-fallback",					/* Key */
		cw_in_generic,					/* get */
		cw_out_generic					/* put */
	}
	,
	{
		"Radio Operational State",			/* name */
		CAPWAP_ELEM_RADIO_OPERATIONAL_STATE,		/* Element ID */
		0, 0,						/* Vendor / Proto */
		3, 3,						/* min/max length */
		radio_operational_state,			/* type */
		"operational-state",				/* Key */
		cw_in_radio_generic_struct,			/* get */
		cw_out_radio_generic_struct			/* put */
	}
	,

	{ 
		"AC Name with Priority",		/* name */
		CAPWAP_ELEM_AC_NAME_WITH_PRIORITY,	/* Element ID */
		0,0,					/* Vendor / Proto */
		1,513,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"ac-name-with-index",			/* Key */
		cw_in_generic_with_index,		/* get */
		cw_out_generic_with_index		/* put */
	}
	,
	{0,0,0,0,0,0,0,0}

};


static cw_State_t discovery_request_states[] = {
	{CAPWAP_STATE_DISCOVERY,CAPWAP_STATE_DISCOVERY},
	{0,0}
};
static struct cw_ElemDef discovery_request_elements[] ={
	{0,0,CAPWAP_ELEM_DISCOVERY_TYPE,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_BOARD_DATA,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_DESCRIPTOR,		1, 0},	
	{0,0,CAPWAP_ELEM_WTP_FRAME_TUNNEL_MODE,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_MAC_TYPE,			1, 0},
	{0,0,CAPWAP_ELEM_MTU_DISCOVERY_PADDING,		0, 0},
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
	{0,0,0,0,0}
};


static cw_State_t discovery_response_states[] = {
	{CAPWAP_STATE_DISCOVERY,CAPWAP_STATE_DISCOVERY},
	{0,0}
};
static struct cw_ElemDef discovery_response_elements[] ={
	{0,0,CAPWAP_ELEM_AC_DESCRIPTOR,			1, 0},
	{0,0,CAPWAP_ELEM_AC_NAME,			1, 0},
	{0,0,CAPWAP_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS,	1, 0},
	{0,0,CAPWAP_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS,	1, 0},
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
	{0,0,0,0,0}

};

static cw_State_t join_request_states[] = {
	{CAPWAP_STATE_JOIN, CAPWAP_STATE_JOIN},
	{0,0}
};
static struct cw_ElemDef join_request_elements[] ={
	{0,0,CAPWAP_ELEM_LOCATION_DATA,			1, 0},
	{0,0,CAPWAP_ELEM_WTP_BOARD_DATA,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_DESCRIPTOR,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_NAME,			1, 0},
	{0,0,CAPWAP_ELEM_SESSION_ID,			1, 0},
	{0,0,CAPWAP_ELEM_WTP_FRAME_TUNNEL_MODE,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_MAC_TYPE,			1, 0},
	{0,0,CAPWAP_ELEM_ECN_SUPPORT,			1, 0},
	{0,0,CAPWAP_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS,	1, 0},
	{0,0,CAPWAP_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS,	1, 0},
	
	{0,0,CAPWAP_ELEM_MAXIMUM_MESSAGE_LENGTH,	0, 0},
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},

	{0,0,0,0,0}
};

static cw_State_t join_response_states[] = {
	{CAPWAP_STATE_JOIN,CAPWAP_STATE_JOIN},
	{0,0}
};
static struct cw_ElemDef join_response_elements[] ={
	{0,0,CAPWAP_ELEM_RESULT_CODE,			1, 0},
	{0,0,CAPWAP_ELEM_AC_DESCRIPTOR,			1, 0},
	{0,0,CAPWAP_ELEM_AC_NAME,			1, 0},
	{0,0,CAPWAP_ELEM_ECN_SUPPORT,			1, 0},
	{0,0,CAPWAP_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS,	1, 0},
	{0,0,CAPWAP_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS,	1, 0},
	{0,0,CAPWAP_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS,	1, 0},
	{0,0,CAPWAP_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS,	1, 0},

	{0,0,CAPWAP_ELEM_MAXIMUM_MESSAGE_LENGTH,	0, 0},
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},

	{0,0,0,0,0}

};


static cw_State_t configuration_status_request_states[] = {
	{CAPWAP_STATE_JOIN, CAPWAP_STATE_CONFIGURE},
	{0,0}
};
static struct cw_ElemDef configuration_status_request_elements[] ={
	{0,0,CAPWAP_ELEM_AC_NAME,			1, 0},
	{0,0,CAPWAP_ELEM_RADIO_ADMINISTRATIVE_STATE,	1, 0},
	{0,0,CAPWAP_ELEM_STATISTICS_TIMER,		1, 0},
	{0,0,CAPWAP_ELEM_WTP_REBOOT_STATISTICS,		1, 0},
	
	{0,0,CAPWAP_ELEM_AC_NAME_WITH_PRIORITY,		0, 0},
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
	{0,0,0,0,0}
};

static cw_State_t configuration_status_response_states[] = {
	{CAPWAP_STATE_JOIN,CAPWAP_STATE_JOIN},
	{0,0}
};
static struct cw_ElemDef configuration_status_response_elements[] ={
	{0,0,CAPWAP_ELEM_CAPWAP_TIMERS,				1, 0},
	{0,0,CAPWAP_ELEM_DECRYPTION_ERROR_REPORT_PERIOD,	1, 0},
	{0,0,CAPWAP_ELEM_IDLE_TIMEOUT,				1, 0},
	{0,0,CAPWAP_ELEM_WTP_FALLBACK,				1, 0},
	
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,		0, CW_IGNORE},
	{0,0,0,0,0}
};


static cw_State_t configuration_update_request_states[] = {
	{CAPWAP_STATE_RUN,CAPWAP_STATE_RUN},
	{0,0}
};
static struct cw_ElemDef configuration_update_request_elements[] ={

	{0,0,CAPWAP_ELEM_RADIO_ADMINISTRATIVE_STATE,	0, 0},
	{0,0,CAPWAP_ELEM_STATISTICS_TIMER,		0, 0},

	
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
	{0,0,0,0,0}
};




static cw_State_t configuration_update_response_states[] = {
	{CAPWAP_STATE_RUN,CAPWAP_STATE_RUN},
	{0,0}
};
static struct cw_ElemDef configuration_update_response_elements[] ={
	{0,0,CAPWAP_ELEM_RESULT_CODE,				1, 0},

	{0,0,0,0,0}
};

static cw_State_t change_state_event_request_states[] = {
	{CAPWAP_STATE_CONFIGURE,CAPWAP_STATE_DATA_CHECK},
	{CAPWAP_STATE_RUN,CAPWAP_STATE_RUN},
	{0,0}
};
static struct cw_ElemDef change_state_event_request_elements[] ={

	{0,0,CAPWAP_ELEM_RADIO_OPERATIONAL_STATE,	1,0},
	{0,0,CAPWAP_ELEM_RESULT_CODE,			1,0},
	
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
	{0,0,0,0,0}
};

static cw_State_t change_state_event_response_states[] = {
	{CAPWAP_STATE_JOIN,CAPWAP_STATE_JOIN},
	{0,0}
};
static struct cw_ElemDef change_state_event_response_elements[] ={
	{0,0,CAPWAP_ELEM_RESULT_CODE,				1, 0},

	{0,0,0,0,0}
};


static cw_State_t wtp_event_request_states[] = {
	{CAPWAP_STATE_RUN,CAPWAP_STATE_RUN},
	{0,0}
};
static struct cw_ElemDef wtp_event_request_elements[] ={

	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
	{0,0,0,0,0}
};

static cw_State_t wtp_event_response_states[] = {
	{CAPWAP_STATE_JOIN,0},
	{0,0}
};
static struct cw_ElemDef wtp_event_response_elements[] ={
	{0,0,CAPWAP_ELEM_RESULT_CODE,				1, 0},

	{0,0,0,0,0}
};


static cw_State_t echo_request_states[] = {
	{CAPWAP_STATE_RUN,CAPWAP_STATE_RUN},
	{0,0}
};
static struct cw_ElemDef echo_request_elements[] ={
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
	{0,0,0,0,0}
};

static cw_State_t echo_response_states[] = {
	{CAPWAP_STATE_RUN,0},
	{0,0}
};
static struct cw_ElemDef echo_response_elements[] ={
	{0,0,CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD,	0, CW_IGNORE},
	{0,0,0,0,0}
};




static struct cw_MsgDef messages[] = {
	{
		"Discovery Request",
		CAPWAP_MSG_DISCOVERY_REQUEST,
		CW_ROLE_AC,
		discovery_request_states,
		discovery_request_elements
	},
	
	{
		"Discovery Response",
		CAPWAP_MSG_DISCOVERY_RESPONSE,
		CW_ROLE_WTP,
		discovery_response_states,
		discovery_response_elements
	},

	{
		"Join Request",
		CAPWAP_MSG_JOIN_REQUEST,
		CW_ROLE_AC,
		join_request_states,
		join_request_elements,
		NULL,
		postprocess_join_request,
	},

	{
		"Join Response",
		CAPWAP_MSG_JOIN_RESPONSE,
		CW_ROLE_WTP,
		join_response_states,
		join_response_elements
	},

	{
		"Configuration Status Request",			/* name */
		CAPWAP_MSG_CONFIGURATION_STATUS_REQUEST,	/* msg type */
		CW_ROLE_AC,					/* role */
		configuration_status_request_states,		/* allowed states */
		configuration_status_request_elements,		/* msg elements */
		NULL,
		NULL,
	},
	
	{
		"Configuration Status Response",		/* name */
		CAPWAP_MSG_CONFIGURATION_STATUS_RESPONSE,	/* msg type */
		CW_ROLE_WTP,					/* role */
		configuration_status_response_states,		/* allowed states */
		configuration_status_response_elements		/* msg elements */
	},

	{
		"Configuration Update Request",			/* name */
		CAPWAP_MSG_CONFIGURATION_UPDATE_REQUEST,	/* msg type */
		CW_ROLE_WTP,					/* role */
		configuration_update_request_states,		/* allowed states */
		configuration_update_request_elements		/* msg elements */
	},

	{
		"Configuration Update Reponse",			/* name */
		CAPWAP_MSG_CONFIGURATION_UPDATE_RESPONSE,	/* msg type */
		CW_ROLE_AC,					/* role */
		configuration_update_response_states,		/* allowed states */
		configuration_update_response_elements		/* msg elements */
	},

	{
		"Change State Event Request",			/* name */
		CAPWAP_MSG_CHANGE_STATE_EVENT_REQUEST,		/* msg type */
		CW_ROLE_AC,					/* role */
		change_state_event_request_states,		/* allowed states */
		change_state_event_request_elements,		/* msg elements */
		NULL,
		NULL,
	},

	{
		"Change State Event Reponse",			/* name */
		CAPWAP_MSG_CHANGE_STATE_EVENT_RESPONSE,		/* msg type */
		CW_ROLE_WTP,					/* role */
		change_state_event_response_states,		/* allowed states */
		change_state_event_response_elements		/* msg elements */
	},

	{
		"WTP Event Request",			/* name */
		CAPWAP_MSG_WTP_EVENT_REQUEST,		/* msg type */
		CW_ROLE_AC,					/* role */
		wtp_event_request_states,		/* allowed states */
		wtp_event_request_elements		/* msg elements */
	},

	{
		"WTP Event Reponse",			/* name */
		CAPWAP_MSG_WTP_EVENT_RESPONSE,		/* msg type */
		CW_ROLE_WTP,					/* role */
		wtp_event_response_states,		/* allowed states */
		wtp_event_response_elements		/* msg elements */
	},


	{
		"Echo Request",			/* name */
		CAPWAP_MSG_ECHO_REQUEST,	/* msg type */
		CW_ROLE_AC,					/* role */
		echo_request_states,		/* allowed states */
		echo_request_elements		/* msg elements */
	},
	
	{
		"Echo Response",		/* name */
		CAPWAP_MSG_ECHO_RESPONSE,	/* msg type */
		CW_ROLE_WTP,					/* role */
		echo_response_states,		/* allowed states */
		echo_response_elements		/* msg elements */
	},


	{0,0,0,0}

};


static int postprocess_join_request(struct conn *conn)
{
	cw_KTV_t * result;
	
	result = cw_ktv_get(conn->remote_cfg,"session-id",CW_TYPE_BSTR16);
	if (result != NULL){
		conn->session_id = result->val.ptr;
		connlist_add_by_session_id(conn->connlist,conn);
	}

	return 1;
}


static cw_StateMachineState_t statemachine_states[]={
	{
		CAPWAP_STATE_DTLS_SETUP, CAPWAP_STATE_JOIN,	/* transition */
		"wait-join",CAPWAP_WAIT_JOIN,			/* timer */
		1						/* retval */
	}
	,
	{
		CAPWAP_STATE_JOIN,CAPWAP_STATE_TIMEOUT,
		NULL,0,
		0, "WTP Join timeout"
	}
	,
	{
		CAPWAP_STATE_JOIN, CAPWAP_STATE_JOIN,
		NULL,0,
		1, "WTP has joined"
		
	}
	,
	{
		CAPWAP_STATE_JOIN, CAPWAP_STATE_CONFIGURE,
		"capwap-timers/change-state-pending-timer",CAPWAP_TIMER_CHANGE_STATE_PENDING_TIMER,
		1, NULL
	}
	,
	{
		CAPWAP_STATE_CONFIGURE,CAPWAP_STATE_TIMEOUT,
		NULL,0,
		0, "WTP change state timeout"
	}
	,
	{
		CAPWAP_STATE_CONFIGURE,CAPWAP_STATE_DATA_CHECK,
		"capwap-timers/data-check-timer",CAPWAP_TIMER_DATA_CHECK_TIMER,
		1, NULL
	}
	,
	{
		CAPWAP_STATE_DATA_CHECK,CAPWAP_STATE_TIMEOUT,
		NULL,0,
		0, "WTP data check timeout"
	}
	,
	
	{
		CAPWAP_STATE_DATA_CHECK,CAPWAP_STATE_RUN,
		"capwap-timers/echo-interval",CAPWAP_ECHO_INTERVAL,
		1, NULL
	}
	,
	{
		CAPWAP_STATE_RUN,CAPWAP_STATE_RUN,
		"capwap-timers/echo-interval",CAPWAP_ECHO_INTERVAL,
		1, NULL
	}
	,
	{
		CAPWAP_STATE_RUN,CAPWAP_STATE_TIMEOUT,
		NULL,0,
		0, "WTP echo timout"
	}
	,
	{0,0,0}
};



struct cw_MsgSet * capwap_register_msg_set(struct cw_MsgSet * set, int mode){
	if (mode != CW_MOD_MODE_CAPWAP)
		return NULL;
	
	cw_dbg(DBG_INFO,"CAPWAP: Register messages");
	
	cw_msgset_add(set,messages, handlers);
	
	cw_msgset_add_states(set,statemachine_states);
	
	mavl_add_ptr(set->types_tree,CW_TYPE_BSTR16);
	mavl_add_ptr(set->types_tree,CW_TYPE_BYTE);
	mavl_add_ptr(set->types_tree,CW_TYPE_DWORD);
	mavl_add_ptr(set->types_tree,CW_TYPE_WORD);

	cw_dbg(DBG_INFO,"CAPWAP: Done register messages");
	return set;
}

void test_sets(){

	struct cw_MsgSet * set = cw_msgset_create();
	if (set==NULL){
		cw_log(LOG_ERR,"No mem");
		return;
	}
	
	cw_msgset_add(set,messages, handlers);

	
}
/*
	cw_msgset_add(set,messages);
	
	cw_elem_handler_t el; 
	memset(&el,0,sizeof(el));
	el.id=CAPWAP_ELEM_DISCOVERY_TYPE;
	
	cw_ElemDef_t search, *result;;
	search.handler = &el;
	
	
	result = mavl_find(set->all_elems,&search);
	
	if (result!=NULL){
		printf("Found: %d %s %d\n",
			result->handler->id,
			result->handler->name,
			result->mand
		);
	}
	else{
		printf("not found\n");
	}

	mlist_t m;
	m = cw_msgset_get_msg(set,CAPWAP_MSG_DISCOVERY_REQUEST);
	printf("Hey: %p\n",m);	
	
	struct mlist_elem * i;
	mlist_foreach(i,m){
		cw_ElemDef_t * le = i->data;
		printf("Element: %d %s\n",le->handler->id, le->handler->name, le->mand);
	}
	



}






*/





