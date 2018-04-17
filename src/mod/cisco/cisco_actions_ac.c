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
#include "cw/dbg.h"
#include "cw/format.h"

static int postprocess_discovery();



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
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_ap_uptime[] = {
	{CW_TYPE_DWORD, "current-uptime", 4,-1},
	{CW_TYPE_DWORD, "last-uptime", 4,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_ap_username_and_password[] = {
	{CW_TYPE_STR,	"username",		33,	-1	},
	{CW_TYPE_STR,	"password",		121,	-1	},
	{CW_TYPE_STR,	"enable-password",	121,	33+121	},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_loghost_config[] = {
	{CW_TYPE_IPADDRESS,	"loghost",		4,	-1},
	{CW_TYPE_STR,		"last-joined-ap",	32,	-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_ap_led_state_config[] = {
	{CW_TYPE_BYTE,		"led-state",		1,	-1},
	{CW_TYPE_BYTE,		"save-flag",		1,	-1},
	{NULL,NULL,0,0}
};

static cw_KTVEnum_t cisco_ap_telnet_ssh[] ={
	{0,	"telnet",	CW_TYPE_BOOL, cw_in_generic },
	{1,	"ssh",		CW_TYPE_BOOL, cw_in_generic },
	{0,0,0,0}
};

static cw_KTVStruct_t cisco_multi_domain_cabability[]={
	{CW_TYPE_BYTE,		"reserved",		1,	-1},
	{CW_TYPE_WORD,		"first-channel",	2,	-1},
	{CW_TYPE_WORD,		"number-of-channels",	2,	-1},
	{CW_TYPE_WORD,		"max-tx-power-level",	2,	-1},
	{NULL,NULL,0,0}
};


static cw_KTVStruct_t cisco_wtp_board_data[]={
	{CW_TYPE_WORD,		"card-id",		2,	-1},
	{CW_TYPE_WORD,		"card-revision",	2,	-1},
	{CW_TYPE_DWORD,		"wtp-model-lo",		4,	-1},
	{CW_TYPE_DWORD,		"wtp-model-hi",		4,	-1},
	{CW_TYPE_STR,		"wtp-serial-number",	16,	-1},
	
	{CW_TYPE_BSTR16,	"ethernet-mac-address",	6,	40},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_ap_led_flash_config[]={
	{CW_TYPE_BYTE,		"flash-enable",		1,	0},
	{CW_TYPE_DWORD,		"flash-sec",		4,	4},
	{CW_TYPE_BYTE,		"save-flag",		4,	8},

	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_ap_static_ip_addr[]={
	{CW_TYPE_IPADDRESS,"address",	4,-1},
	{CW_TYPE_IPADDRESS,"netmask",	4,-1},
	{CW_TYPE_IPADDRESS,"gateway",	4,-1},
	{CW_TYPE_IPADDRESS,"unknown",	4,-1},
	{CW_TYPE_BOOL,"enabled",	1,-1},
	{NULL,NULL,0,0}
};


static cw_KTVStruct_t cisco_ap_regulatory_domain4[]={
	{CW_TYPE_BOOL,"set",1,-1},
	{CW_TYPE_BYTE,"slot",1,-1},
	{CW_TYPE_BYTE,"code0",1,-1},
	{CW_TYPE_BYTE,"code1",1,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_ap_regulatory_domain5[]={
	{CW_TYPE_BYTE,"band-id",1,-1},
	{CW_TYPE_BOOL,"set",1,-1},
	{CW_TYPE_BYTE,"slot",1,-1},
	{CW_TYPE_BYTE,"code0",1,-1},
	{CW_TYPE_BYTE,"code1",1,-1},
	{NULL,NULL,0,0}
};


int cisco_in_ap_regulatory_domain(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
	char key[CW_KTV_MAX_KEY_LEN];
	int idx;
	void * type;

	idx = cw_ktv_idx_get(params->conn->remote_cfg,eh->key);

	sprintf(key,"%s.%d",eh->key,idx+1);
	
	if(len==4)
		type = cisco_ap_regulatory_domain4;
	if(len==5)
		type = cisco_ap_regulatory_domain5;
	cw_ktv_read_struct(params->conn->remote_cfg,type,key,data,len);

	return 1;
}


int cisco_out_ap_regulatory_domain(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)

{
	char key[CW_KTV_MAX_KEY_LEN];
	char testkey[CW_KTV_MAX_KEY_LEN];
	int idx;
	void * type;
	cw_KTV_t * result, search;
	int len,start;
	uint8_t * ob;


	idx = 0;
	ob = dst;

	do {
		sprintf(key,"%s.%d",eh->key,idx);
		search.key=key;
		result = mavl_get_first(params->conn->local_cfg,&search);
		if (result==NULL)
			break;
		if (strncmp(result->key,key,strlen(key))!=0)
			break;
		
		sprintf(testkey,"%s/%s",key,"band-id");
		result = cw_ktv_get(params->conn->local_cfg,key,CW_TYPE_BYTE);
		if (result==NULL){
			type = cisco_ap_regulatory_domain4;
		}
		else{
			type = cisco_ap_regulatory_domain5;
		}
		
		start = params->conn->header_len(eh);
		len = cw_ktv_write_struct(params->conn->local_cfg,type,key,ob+start);
		ob += params->conn->write_header(eh,ob,len);
		
		idx++;
		
	}while(1);


	sprintf(key,"%s.%d",eh->key,idx+1);
/*	if(len==4)
		type = cisco_ap_regulatory_domain4;
	if(len==5)
		type = cisco_ap_regulatory_domain5;
	cw_ktv_read_struct(params->conn->remote_cfg,type,key,data,len);
*/
	return ob-dst;
}

static cw_KTVStruct_t cisco_ap_model[]={
	{CW_TYPE_STR,"model",30,-1},
	{CW_TYPE_STR,"image",30,30},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_wtp_radio_config[]={
	{CW_TYPE_BYTE,"cfg-type",1,-1},
	{CW_TYPE_WORD,"occupancy-limit",2,-1},
	{CW_TYPE_BYTE,"cfg-period",1,-1},
	{CW_TYPE_WORD,"cfp-maximum-duration",2,-1},
	{CW_TYPE_BSTR16,"bss-id",6,-1},
	{CW_TYPE_WORD,"beacon-period",2,-1},
	{CW_TYPE_BSTR16,"country-str1",3,-1},
	{CW_TYPE_BSTR16,"country-str2",3,-1},
	{CW_TYPE_BYTE,"gpr-period",1,-1},
	{CW_TYPE_DWORD,"reg",4,-1},
	{CW_TYPE_BYTE,"max-stations",1,-1},
	{NULL,NULL,0,0}
};

static struct cw_ElemHandler handlers[] = {
	
	{ 
		"AC Name -(zero-length allowed)",	/* name */
		CAPWAP_ELEM_AC_NAME,			/* Element ID */
		0,0,					/* Vendor / Proto */
		0,CAPWAP_MAX_AC_NAME_LEN,		/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"ac-name",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	
	
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
		"80211 WTP Radio Information - Cisco",		/* name */
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
		"cisco/lw-path-mtu",			/* Key */
		cw_in_generic_struct,			/* get */
		cisco_out_lw_path_mtu			/* put */
	}
	,
	
	{ 
		"AP Uptime",				/* name */
		CISCO_ELEM_AP_UPTIME,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		8,8,					/* min/max length */
		cisco_ap_uptime,			/* type */
		"cisco/ap-uptime",			/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	}
	,
	
	{ 
		"AP Username and Password",		/* name */
		CISCO_LWELEM_AP_USERNAME_PASSWORD,	/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		0,0,					/* min/max length */
		cisco_ap_username_and_password,		/* type */
		"cisco/ap-username-and-password",	/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	}
	,
	{ 
		"Loghost Config",			/* name */
		CISCO_LWELEM_AP_LOGHOST_CONFIG,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		36,36,					/* min/max length */
		cisco_loghost_config,			/* type */
		"cisco/loghost-config",			/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	}
	,
	{ 
		"AP LED State Config",			/* name */
		CISCO_ELEM_AP_LED_STATE_CONFIG,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		2,2,					/* min/max length */
		cisco_ap_led_state_config,		/* type */
		"cisco/ap-led-state-config",		/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	}
	,
	{ 
		"AP Telnet SSH",			/* name */
		CISCO_LWELEM_AP_TELNET_SSH,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		2,2,					/* min/max length */
		cisco_ap_telnet_ssh,			/* type */
		"cisco/ap-telnet-ssh",			/* Key */
		cw_in_generic_enum,			/* get */
		NULL					/* put */
	}
	,
	{ 
		"AP Log Facility",			/* name */
		CISCO_ELEM_AP_LOG_FACILITY,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		"cisco/ap-log-facility",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,
	{ 
		"802.11 Multi Domain Capability",	/* name */
		CISCO_ELEM_MULTI_DOMAIN_CAPABILITY,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		8,8,					/* min/max length */
		cisco_multi_domain_cabability,			/* type */
		"cisco-multi-domain-capability",	/* Key */
		cw_in_radio_generic_struct,		/* get */
		NULL					/* put */
	}
	,
	
	{ 
		"Cisco WTP Board Data",			/* name */
		CISCO_ELEM_WTP_BOARD_DATA,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		8,48,					/* min/max length */
		cisco_wtp_board_data,			/* type */
		"cisco/wtp-board-data",			/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct,			/* put */
	},
	{ 
		"AP LED Flash Config",			/* name */
		CISCO_ELEM_AP_LED_FLASH_CONFIG,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		0,48,					/* min/max length */
		cisco_ap_led_flash_config,		/* type */
		"cisco/ap-led-flash-config",		/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	},

	{ 
		"AP Pre Std Switch Config",		/* name */
		CISCO_ELEM_AP_PRE_STD_SWITCH_CONFIG,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		"cisco/ap-pre-std-switch-config",	/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ 
		"AP Power Injector Config",		/* name */
		CISCO_ELEM_AP_POWER_INJECTOR_CONFIG,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		8,8,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/ap-power-injector-config",	/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ 
		"AP Mode And Type",			/* name */
		CISCO_ELEM_AP_MODE_AND_TYPE,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		2,2,					/* min/max length */
		CW_TYPE_WORD,				/* type */
		"cisco/ap-mode-and-type",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ 
		"AP Sub Mode",				/* name */
		CISCO_LWELEM_AP_SUBMODE,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		"cisco/ap-sub-mode",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},
	{ 
		"AP Static IP Address",			/* name */
		CISCO_ELEM_AP_STATIC_IP_ADDR,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		17,17,					/* min/max length */
		cisco_ap_static_ip_addr,		/* type */
		"cisco/ap-satic-ip-addr",		/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	},

	{ 
		"AP Min IOS Version",			/* name */
		CISCO_ELEM_AP_MIN_IOS_VERSION,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		0,32,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/ap-min-ios-version",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ 
		"AP Backup Software Version",		/* name */
		CISCO_ELEM_AP_BACKUP_SOFTWARE_VERSION,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		0,32,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/ap-backup-software-version",	/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},
	
	{ 
		"AP Ethernet Port Type",		/* name */
		CISCO_LWELEM_AP_ETHERNET_PORT_SUBTYPE,	/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		3,3,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/ap-ethernet-port-type",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ /* it's wrong to store the reg domain in radio/xy/...
	   and has to be corected in the future */
	 
		"AP Regulatory Domain",			/* name */
		CISCO_ELEM_AP_REGULATORY_DOMAIN,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		4,5,					/* min/max length */
		cisco_ap_regulatory_domain4,		/* type */
		"cisco/ap-regulatory-domain",		/* Key */
		cisco_in_ap_regulatory_domain,		/* get */
		cisco_out_ap_regulatory_domain		/* put */
	},
	
	{ 
		"AP Model",				/* name */
		CISCO_ELEM_AP_MODEL,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		60,60,					/* min/max length */
		cisco_ap_model,				/* type */
		"cisco/ap-model",			/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct,			/* put */
	},
	
	{ 
		"Reset Button State",			/* name */
		CISCO_ELEM_RESET_BUTTON_STATE,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BOOL,				/* type */
		"cisco/reset-button-state",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ 
		"WTP Radio Configuration",		/* name */
		CISCO_ELEM_WTP_RADIO_CONFIGURATION,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		27,27,					/* min/max length */
		cisco_wtp_radio_config,			/* type */
		"cisco/wtp-radio-config",		/* Key */
		cw_in_radio_generic_struct,		/* get */
		cw_out_radio_generic_struct		/* put */
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

static int join_response_states[] = {CAPWAP_STATE_JOIN,0};
static struct cw_ElemDef join_response_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PATH_MTU,	0, 0},

	{0,0,			CAPWAP_ELEM_ECN_SUPPORT,	0, CW_DELETE},

	{0,0,0,00}
	
};


static int configuration_status_request_states[] = {CAPWAP_STATE_JOIN,0};
static struct cw_ElemDef configuration_status_request_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,1, CW_IGNORE},
	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_UPTIME,			0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_LED_STATE_CONFIG,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_LOG_FACILITY,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_MULTI_DOMAIN_CAPABILITY,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_WTP_BOARD_DATA,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_LED_FLASH_CONFIG,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_PRE_STD_SWITCH_CONFIG,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_POWER_INJECTOR_CONFIG,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_MODE_AND_TYPE,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_STATIC_IP_ADDR,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_MIN_IOS_VERSION,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_BACKUP_SOFTWARE_VERSION,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_REGULATORY_DOMAIN,	1, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_MODEL,			1, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_RESET_BUTTON_STATE,		1, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_WTP_RADIO_CONFIGURATION,	1, 0},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_USERNAME_PASSWORD,	1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_LOGHOST_CONFIG,		1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_TELNET_SSH,		1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_SUBMODE,		1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_ETHERNET_PORT_SUBTYPE,	1, 0},
	
	{0,0,0,00}
	
};

static int configuration_status_response_states[] = {CAPWAP_STATE_JOIN,0};
static struct cw_ElemDef configuration_status_response_elements[] ={
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,1, CW_IGNORE},
	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_MULTI_DOMAIN_CAPABILITY,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_WTP_RADIO_CONFIGURATION,	1, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_LED_STATE_CONFIG,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_LOG_FACILITY,		0, 0},
	
	{0,0,0,0}
};

static struct cw_MsgDef messages[] = {
	{
		NULL,				/* name */
		CAPWAP_MSG_DISCOVERY_REQUEST,	/* type */
		CW_ROLE_AC,			/* role */
		discovery_request_states,	/* states */
		discovery_request_elements,	/* elements */
		postprocess_discovery		/* postprocess fun */
	},
	{
		NULL,				/* name */
		CAPWAP_MSG_DISCOVERY_RESPONSE,	/* type */
		CW_ROLE_WTP,
		discovery_response_states,
		discovery_response_elements,
		
	},
	{
		NULL,				/* name */
		CAPWAP_MSG_JOIN_REQUEST,	/* type */
		CW_ROLE_AC,
		join_request_states,
		join_request_elements,
		postprocess_discovery		/* postprocess */
	},
	{
		NULL,				/* name */
		CAPWAP_MSG_JOIN_RESPONSE,	/* type */
		CW_ROLE_WTP,
		join_response_states,
		join_response_elements
	},


	{
		NULL,						/* name */
		CAPWAP_MSG_CONFIGURATION_STATUS_REQUEST,	/* type */
		CW_ROLE_AC,
		configuration_status_request_states,
		configuration_status_request_elements,
		NULL					/* postprocess */
	},

	{
		NULL,						/* name */
		CAPWAP_MSG_CONFIGURATION_STATUS_RESPONSE,	/* type */
		CW_ROLE_WTP,
		configuration_status_response_states,
		configuration_status_response_elements,
		NULL					/* postprocess */
	},


	{0,0,0,0}

};



struct cw_MsgSet * cisco_register_msg_set(struct cw_MsgSet * set, int mode){
        if (mode != CW_MOD_MODE_CAPWAP)
                return NULL;
        cw_msgset_add(set,messages, handlers);
        return set;
}



static void set_ac_version(struct conn * conn)
{
	cw_KTV_t * wtpver;
	char verstr[512];
	wtpver = cw_ktv_get(conn->remote_cfg,"wtp-descriptor/software/version", CW_TYPE_BSTR16);
	if (wtpver){
		
		cw_format_version(verstr,wtpver->type->data(wtpver),wtpver->type->len(wtpver));
		
		cw_dbg(DBG_INFO, "Cisco - Setting AC software version to: %s", verstr);
		
		mavl_del(conn->local_cfg,&wtpver);
		cw_ktv_add(conn->local_cfg,"ac-descriptor/software/version",CW_TYPE_BSTR16,
			wtpver->type->data(wtpver),wtpver->type->len(wtpver));
	}

}

static int postprocess_discovery(struct conn *conn)
{
	if (conn->role != CW_ROLE_AC )
		return 0;
	set_ac_version(conn);
	cw_detect_nat(conn);
	return 1;
}
