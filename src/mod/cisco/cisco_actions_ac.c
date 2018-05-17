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
static int preprocess_join_request();
static int postprocess_join_request();


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

static cw_KTVStruct_t cisco_login[] = {
	{CW_TYPE_STR,	"username",		33,	-1	},
	{CW_TYPE_STR,	"password",		121,	-1	},
	{CW_TYPE_STR,	"enable-password",	121,	33+121	},
	{CW_TYPE_WORD,	"option",		2,	275	},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_8021xlogin[] = {
	{CW_TYPE_STR,	"username",		33,	-1	},
	{CW_TYPE_STR,	"password",		121,	-1	},
	{CW_TYPE_WORD,	"option",		2,	275	},
	{NULL,NULL,0,0}
};


static cw_KTVEnum_t cisco_ap_username_and_password_enum[] ={
	{2,	"802.1x-credentials",	cisco_8021xlogin, cw_in_generic_struct, cw_ktv_write_struct },

	{1,	"login-credentials",	cisco_login, cw_in_generic_struct, cw_ktv_write_struct },

	{0,0,0,0}
};


static cw_KTVIndexed_t cisco_ap_username_and_password = {
	276,cisco_ap_username_and_password_enum
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

static cw_KTVEnum_t cisco_ap_telnet_ssh_enum[] ={
	{0,	"telnet",	CW_TYPE_BOOL, cw_in_generic, NULL },
	{1,	"ssh",		CW_TYPE_BOOL, cw_in_generic, NULL },
	{0,0,0,0}
};

static cw_KTVIndexed_t cisco_ap_telnet_ssh = {
	1,cisco_ap_telnet_ssh_enum
};

static cw_KTVStruct_t cisco_multi_domain_cabability[]={
	{CW_TYPE_BYTE,		"reserved",		1,	-1},
	{CW_TYPE_WORD,		"first-channel",	2,	-1},
	{CW_TYPE_WORD,		"number-of-channels",	2,	-1},
	{CW_TYPE_WORD,		"max-tx-power-level",	2,	-1},
	{NULL,NULL,0,0}
};




static cw_KTVStruct_t cisco_wtp_board_data[]={
	{CW_TYPE_WORD,		"card-id",			2,	-1},
	{CW_TYPE_WORD,		"card-revision",		2,	-1},
	{CW_TYPE_DWORD,		"wtp-model-lo",			4,	-1},
	{CW_TYPE_DWORD,		"wtp-model-hi",			4,	-1},
	{CW_TYPE_STR,		"wtp-serial-number",		16,	-1},
	{CW_TYPE_BYTE,		"options/ant-type",		1,	36},
	{CW_TYPE_BYTE,		"options/flex-connect",		1,	-1},
	{CW_TYPE_BYTE,		"options/ap-type",		1,	-1},
	{CW_TYPE_BYTE,		"options/failover-priority",	1,	-1},
	{CW_TYPE_BSTR16,	"ethernet-mac-address",		6,	40},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_wtp_board_data_options[]={
	{CW_TYPE_BYTE,		"ant-type",		1,	-1},
	{CW_TYPE_BYTE,		"flex-connect",		1,	-1},
	{CW_TYPE_BYTE,		"ap-type",		1,	-1},
	{CW_TYPE_BYTE,		"failover-priority",	1,	-1},

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

static cw_KTVStruct_t cisco_mac_operation73[]={
	{CW_TYPE_BYTE,"reserved",1,-1},
	{CW_TYPE_WORD,"rts-threshold",2,-1},
	{CW_TYPE_BYTE,"short-retry",1,-1},
	{CW_TYPE_BYTE,"long-retry",1,-1},
	{CW_TYPE_WORD,"fragmentation-threshold",2,-1},
	{CW_TYPE_DWORD,"tx-msdu-lifetime",4,-1},
	{CW_TYPE_DWORD,"rx-msdu-lifetime",4,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_mac_operation75[]={
	{CW_TYPE_WORD,"reserved",2,-1},
	{CW_TYPE_WORD,"rts-threshold",2,-1},
	{CW_TYPE_BYTE,"short-retry",1,-1},
	{CW_TYPE_BYTE,"long-retry",1,-1},
	{CW_TYPE_WORD,"fragmentation-threshold",2,-1},
	{CW_TYPE_DWORD,"tx-msdu-lifetime",4,-1},
	{CW_TYPE_DWORD,"rx-msdu-lifetime",4,-1},
	{NULL,NULL,0,0}
};


static cw_KTVStruct_t cisco_ap_power_injector_config[]={
	{CW_TYPE_BYTE,"state",1,-1},
	{CW_TYPE_BYTE,"selection",1,-1},
	{CW_TYPE_BSTR16,"sitch-mac-address",6,-1},
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
	

	type = NULL;
	result = cw_ktv_get(params->conn->local_cfg,"ac-descriptor/software/version",CW_TYPE_BSTR16);
	if (result!=NULL){
		if(result->type->len(result)==4){
			uint32_t rv;
			rv = cw_get_dword(result->type->data(result));
			if (rv >= 0x07056600){
				type = cisco_ap_regulatory_domain5;
			}
			else{
				type = cisco_ap_regulatory_domain4;
			}
		}
		
	}


	do {
		sprintf(key,"%s.%d",eh->key,idx);
		search.key=key;
		result = mavl_get_first(params->conn->local_cfg,&search);
		if (result==NULL)
			break;
		if (strncmp(result->key,key,strlen(key))!=0)
			break;
		
		if(type == NULL){
			sprintf(testkey,"%s/%s",key,"band-id");
			result = cw_ktv_get(params->conn->local_cfg,key,CW_TYPE_BYTE);
			if (result==NULL){
				type = cisco_ap_regulatory_domain4;
			}
			else{
				type = cisco_ap_regulatory_domain5;
			}
		}
		
		start = params->conn->header_len(eh);
		len = cw_ktv_write_struct(params->conn->local_cfg,NULL,type,key,ob+start);
		ob += params->conn->write_header(eh,ob,len);
		
		idx++;
		
	}while(1);


	sprintf(key,"%s.%d",eh->key,idx+1);
	return ob-dst;
}

static cw_KTVStruct_t cisco_ap_model[]={
	{CW_TYPE_STR,"model",30,-1},
	{CW_TYPE_STR,"image",30,30},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_wtp_radio_config73[]={
	{CW_TYPE_BYTE,"cfg-type",1,-1},
	{CW_TYPE_WORD,"occupancy-limit",2,-1},
	{CW_TYPE_BYTE,"cfg-period",1,-1},
	{CW_TYPE_WORD,"cfp-maximum-duration",2,-1},
	{CW_TYPE_BSTR16,"bss-id",6,-1},
	{CW_TYPE_WORD,"beacon-period",2,-1},
	{CW_TYPE_STR,"country-str1",3,-1},
	{CW_TYPE_STR,"country-str2",3,-1},
	{CW_TYPE_BYTE,"gpr-period",1,-1},
	{CW_TYPE_DWORD,"reg",4,-1},
	{CW_TYPE_BYTE,"max-stations",1,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_wtp_radio_config75[]={
	{CW_TYPE_BYTE,"cfg-type",1,-1},
	{CW_TYPE_WORD,"occupancy-limit",2,-1},
	{CW_TYPE_BYTE,"cfg-period",1,-1},
	{CW_TYPE_WORD,"cfp-maximum-duration",2,-1},
	{CW_TYPE_BSTR16,"bss-id",6,-1},
	{CW_TYPE_WORD,"beacon-period",2,-1},
	{CW_TYPE_STR,"country-str1",3,-1},
	{CW_TYPE_STR,"country-str2",3,-1},
	{CW_TYPE_BYTE,"gpr-period",1,-1},
	{CW_TYPE_DWORD,"reg",4,-1},
	{CW_TYPE_BYTE,"max-stations",1,-1},
	{CW_TYPE_BYTE,"unknown75",1,-1},
	{NULL,NULL,0,0}
};




static cw_KTVStruct_t cisco_tx_power[]={
	{CW_TYPE_BYTE,"reserved",1,-1},
	{CW_TYPE_WORD,"current-tx-power",2,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_ap_qos[]={
	{CW_TYPE_BYTE,"tag-packets",1,-1},
	{CW_TYPE_BYTE,"uranium-queue-depth",1,-1},
	{CW_TYPE_WORD,"uranium-cwmin",2,-1},
	{CW_TYPE_WORD,"uranium-cwmax",2,-1},
	{CW_TYPE_BYTE,"uranium-aifs",1,-1},
	{CW_TYPE_WORD,"uranium-cbr",2,-1},
	{CW_TYPE_BYTE,"uranium-dot1p-tag",1,-1},
	{CW_TYPE_BYTE,"uranium-dscp-tag",1,-1},
	
	{CW_TYPE_BYTE,"platinum-queue-depth",1,-1},
	{CW_TYPE_WORD,"platinum-cwmin",2,-1},
	{CW_TYPE_WORD,"platinum-cwmax",2,-1},
	{CW_TYPE_BYTE,"platinum-aifs",1,-1},
	{CW_TYPE_WORD,"platinum-cbr",2,-1},
	{CW_TYPE_BYTE,"platinum-dot1p-tag",1,-1},
	{CW_TYPE_BYTE,"platinum-dscp-tag",1,-1},

	{CW_TYPE_BYTE,"gold-queue-depth",1,-1},
	{CW_TYPE_WORD,"gold-cwmin",2,-1},
	{CW_TYPE_WORD,"gold-cwmax",2,-1},
	{CW_TYPE_BYTE,"gold-aifs",1,-1},
	{CW_TYPE_WORD,"gold-cbr",2,-1},
	{CW_TYPE_BYTE,"gold-dot1p-tag",1,-1},
	{CW_TYPE_BYTE,"gold-dscp-tag",1,-1},

	{CW_TYPE_BYTE,"silver-queue-depth",1,-1},
	{CW_TYPE_WORD,"silver-cwmin",2,-1},
	{CW_TYPE_WORD,"silver-cwmax",2,-1},
	{CW_TYPE_BYTE,"silver-aifs",1,-1},
	{CW_TYPE_WORD,"silver-cbr",2,-1},
	{CW_TYPE_BYTE,"silver-dot1p-tag",1,-1},
	{CW_TYPE_BYTE,"silver-dscp-tag",1,-1},

	{CW_TYPE_BYTE,"bronze-queue-depth",1,-1},
	{CW_TYPE_WORD,"bronze-cwmin",2,-1},
	{CW_TYPE_WORD,"bronze-cwmax",2,-1},
	{CW_TYPE_BYTE,"bronze-aifs",1,-1},
	{CW_TYPE_WORD,"bronze-cbr",2,-1},
	{CW_TYPE_BYTE,"bronze-dot1p-tag",1,-1},
	{CW_TYPE_BYTE,"bronze-dscp-tag",1,-1},

	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_ap_core_dump[]={
	{CW_TYPE_IPADDRESS,"tftp-server",4,-1},
	{CW_TYPE_BOOL,"compression",1,16},
	{CW_TYPE_STR,"filename",199,17},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_vlan[]={
	{CW_TYPE_BOOL,"tagging",1,-1},
	{CW_TYPE_WORD,"id",2,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_rouge_and_mss[]={
	{CW_TYPE_BOOL,"enable",1,-1},
	{CW_TYPE_WORD,"tcp-adjust-mss",2,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_rouge_detections[]={
	{CW_TYPE_BOOL,"rouge-detection",1,-1},
	{CW_TYPE_BSTR16,"rest",6,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_ap_venue_settings[]={
	{CW_TYPE_WORD,"group",2,-1},
	{CW_TYPE_BYTE,"type",1,-1},
	{CW_TYPE_STR,"language",3,-1},
	{CW_TYPE_STR,"name",-1,7},
	{NULL,NULL,0,0}
	
};

static cw_KTVStruct_t cisco_ap_mode_and_type[]={
	{CW_TYPE_BYTE,"mode",1,-1},
	{CW_TYPE_BYTE,"type",1,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_add_wlan[]={
	{CW_TYPE_BYTE,"radio-id",1,-1},
	{CW_TYPE_WORD,"wlan-capability",2,-1},
	{CW_TYPE_BYTE,"wlan-id",1,-1},
	{CW_TYPE_DWORD,"encryption-policy",4,-1},

	{CW_TYPE_BSTR16,"wep-key",13,9},
	{CW_TYPE_BYTE,"encryption",1,42},
	
	{CW_TYPE_BOOL,"broadcast-ssid",1,426},
	{CW_TYPE_WORD,"session-timout",2,475},
	{CW_TYPE_BYTE, "dtim-period",1,541},
	{CW_TYPE_STR, "ssid-a",30,545},
	{CW_TYPE_BYTE, "allow-aaa-override",1,578},
	{CW_TYPE_BYTE, "max-stations",1,580},
	
	{NULL,NULL,0,0}
};


static int cisco_in_lw_del_wlan(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
	int wlan_id, radio_id;
	char key[CW_KTV_MAX_KEY_LEN];
	
	radio_id=cw_get_byte(data);
	wlan_id=cw_get_word(data+1);
	sprintf(key,"radio.%d/wlan.%d",radio_id,wlan_id);
	cw_ktv_del_sub(params->conn->local_cfg,key);
	cw_dbg(DBG_INFO,"Del WLAN rid=%d, id=%d",wlan_id);
	return 0;
}



static int cisoc_add_wlan_mkkey(const char *pkey, uint8_t*data, int len, char *dst)
{
	int wlan_id,radio_id;
	
	radio_id = cw_get_byte(data);
	wlan_id = cw_get_byte(data+3);
	sprintf(dst,"radio.%d/wlan.%d",radio_id,wlan_id);
	return 1;
}

static cw_KTVStruct_t cisco_add_lwwlan[]={
	{CW_TYPE_BSTR16, "misc", 8, 2},
	{CW_TYPE_STR, "ssid",-1,10},
	{CW_TYPE_WORD, "misc2", 2, 48 },
	{NULL,NULL,0,0}
};

static cw_KTVValRange_t oper_val_state[]={
	{1,1,"disabled"},
	{2,2,"enabled"},
	{0,0,NULL}
};

static cw_KTVValRange_t oper_val_cause[]={
	{0,0,"Normal"},
	{1,1,"Radio Failure"},
	{2,2,"Software Failure"},
	{3,3,"Administratively Set"},
	{0,0,NULL}
};

static cw_KTVStruct_t cisco_radio_oper_state[]={
	{CW_TYPE_BYTE, "state", 1, -1, oper_val_state},
	{CW_TYPE_BYTE, "cause", 1, -1,oper_val_cause},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_capwap_timers[] = {
	{CW_TYPE_BYTE, "max-discovery-interval", 1,-1},
	{CW_TYPE_BYTE, "echo-interval", 1,-1},
	{NULL,NULL,0,0}
};




static int cisoc_add_lwwlan_mkkey(const char *pkey, uint8_t*data, int len, char *dst)
{
	int wlan_id,radio_id;
	
	radio_id = cw_get_byte(data);
	wlan_id = cw_get_byte(data+1);
	sprintf(dst,"radio.%d/wlan.%d",radio_id,wlan_id);
	return 1;
}





static cw_KTVStruct_t cisco_ssc_hash[]={
	{CW_TYPE_BOOL,"validate",1,-1},
	{CW_TYPE_BSTR16,"hash",-1,-1},
	{NULL,NULL,0,0}
};



static cw_KTVStruct_t cisco_hardware_info[]={
	{CW_TYPE_STR, "ram-type",20,-1},
	{CW_TYPE_STR, "cpu",40,40},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_discovery_protocol[]={
	{CW_TYPE_WORD, "data",2,-1},
	{CW_TYPE_BOOL, "enabled",1,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_rad_extended_config[]={
	{CW_TYPE_WORD, "beacon-interval",2,-1},
	{CW_TYPE_WORD, "beacon-range",2,-1},
	{CW_TYPE_WORD, "multicast-buffer",2,-1},
	{CW_TYPE_WORD, "multicast-data-range",2,-1},
	{CW_TYPE_WORD, "rx-sensop-threshold",2,-1},
	{CW_TYPE_WORD, "c-ccat",2,-1},
	{NULL,NULL,0,0}
};


static cw_KTVStruct_t cisco_80211_assoc_limit[]={
	{CW_TYPE_BOOL, "enable",1,-1},
	{CW_TYPE_BYTE, "limit",1,-1},
	{CW_TYPE_WORD, "interval",1,-1},
	{NULL,NULL,0,0}
};

static cw_KTVStruct_t cisco_dot11r_wlc_mac_and_ip[]={
	{CW_TYPE_IPADDRESS, "ip-address",4,-1},
	{CW_TYPE_BSTR16, ",ac-address",6,-1},
	{NULL,NULL,0,0}
};
/*

static int cisco_data(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
	int wlan_id, radio_id;
	char key[CW_KTV_MAX_KEY_LEN];
	
	radio_id=cw_get_byte(data);
	wlan_id=cw_get_word(data+1);
	sprintf(key,"radio.%d/wlan.%d",radio_id,wlan_id);

	read_struct(eh->type,key);

	cw_dbg(DBG_INFO,"Del WLAN rid=%d, id=%d",wlan_id);
	return 0;
}
*/

static struct cw_ElemHandler handlers73[] = {
	
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
		CISCO_ELEM_RAD_NAME,		/* Element ID */
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
		cisco_wtp_board_data_options,	/* type */
		"cisco/wtp-board-data/options",	/* Key */
		cw_in_generic_struct,		/* handler */
		cw_out_generic_struct		/* put */
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
		&cisco_ap_username_and_password,		/* type */
		"cisco/ap-username-and-password",	/* Key */
		cw_in_generic_indexed_enum,		/* get */
		cw_out_generic_indexed_enum		/* put */
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
		&cisco_ap_telnet_ssh,			/* type */
		"cisco/ap-telnet-ssh",			/* Key */
		cw_in_generic_indexed_enum,		/* get */
		cw_out_generic_indexed_enum		/* put */
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
		"cisco/multi-domain-capability",	/* Key */
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
		cisco_ap_power_injector_config,		/* type */
		"cisco/ap-power-injector-config",	/* Key */
		cw_in_generic_struct,				/* get */
		cw_out_generic_struct				/* put */
	},

	{ 
		"AP Mode And Type",			/* name */
		CISCO_ELEM_AP_MODE_AND_TYPE,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		2,2,					/* min/max length */
		cisco_ap_mode_and_type,				/* type */
		"cisco/ap-mode-and-type",		/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
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

	{ /* WTP Radio Configuration for AC/WPT with version 7.3 */
	
		"WTP Radio Configuration",		/* name */
		CISCO_ELEM_WTP_RADIO_CONFIGURATION,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		27,27,					/* min/max length */
		cisco_wtp_radio_config73,		/* type */
		"cisco/wtp-radio-config",		/* Key */
		cw_in_radio_generic_struct,		/* get */
		cw_out_radio_generic_struct		/* put */
	}
	,

	{ 
		"MWAR Hash Value",			/* name */
		CISCO_LWELEM_MWAR_HASH_VALUE,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		1,64,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/mwar-hash-value",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ 
		"Mac Operation",			/* name */
		CISCO_ELEM_MAC_OPERATION,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		16,16,					/* min/max length */
		cisco_mac_operation73,			/* type */
		"cisco/mac-operation",			/* Key */
		cw_in_radio_generic_struct,		/* get */
		cw_out_radio_generic_struct		/* put */
	},

	{ 
		"Tx Power",				/* name */
		CISCO_ELEM_TX_POWER,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		4,4,					/* min/max length */
		cisco_tx_power,				/* type */
		"cisco/tx-power",			/* Key */
		cw_in_radio_generic_struct,		/* get */
		cw_out_radio_generic_struct		/* put */
	},

	{ 
		"AP Quality of Service",		/* name */
		CISCO_ELEM_AP_QOS,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		52,52,					/* min/max length */
		cisco_ap_qos,				/* type */
		"cisco/ap-qos",				/* Key */
		cw_in_radio_generic_struct,		/* get */
		cw_out_radio_generic_struct		/* put */
	},

	{ 
		"Air Space Capability",			/* name */
		CISCO_ELEM_AIRSPACE_CAPABILITY,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		2,2,					/* min/max length */
		CW_TYPE_BYTE,				/* type */
		"cisco/air-space-capability",		/* Key */
		cw_in_radio_generic,				/* get */
		cw_out_radio_generic				/* put */
	},

	{ 
		"Location Data",			/* name */
		CISCO_ELEM_LOCATION_DATA,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		0,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"location-data",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ 
		"AP Core Dump",			/* name */
		CISCO_ELEM_AP_CORE_DUMP,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		0,1024,					/* min/max length */
		cisco_ap_core_dump,				/* type */
		"cisco/ap-core-dump",			/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	},
	
	{ 
		"Statitsics Timer",			/* name */
		CISCO_ELEM_STATISTICS_TIMER,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		2,2,					/* min/max length */
		CW_TYPE_WORD,				/* type */
		"statistics-timer",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,
	
	{ 
		"AC Name with Index",			/* name */
		CISCO_ELEM_AC_NAME_WITH_INDEX,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,513,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"ac-name-with-index",			/* Key */
		cw_in_generic_with_index,		/* get */
		cw_out_generic_with_index		/* put */
	}
	,
	
	{ 
		"AC IP Address with Index",		/* name */
		CISCO_LWELEM_AC_IP_ADDR_WITH_INDEX,	/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		5,5,					/* min/max length */
		CW_TYPE_IPADDRESS,			/* type */
		"cisco/ac-ip-addr-with-index",		/* Key */
		cw_in_generic_with_index,		/* get */
		cw_out_generic_with_index		/* put */
	}
	,
	{ 
		"AP Failover Priority",					/* name */
		CISCO_LWELEM_AP_FAILOVER_PRIORITY,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,			/* Vendor / Proto */
		1,1,							/* min/max length */
		CW_TYPE_BYTE,						/* type */
		"cisco/wtp-board-data/options/failover-priority",	/* Key */
		cw_in_generic,						/* get */
		cw_out_generic						/* put */
	}
	,

	{ 
		"VLAN",					/* name */
		CISCO_LWELEM_VLAN,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		3,3,					/* min/max length */
		cisco_vlan,				/* type */
		"cisco/vlan",				/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	}
	,


	{ 
		"TCP Adjust MSS",				/* name */
		CISCO_LWELEM_TCP_ADJUST_MSS,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		3,3,					/* min/max length */
		cisco_rouge_and_mss,				/* type */
		"cisco/rouge-and-mss",				/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	}
	,

	{ 
		"AP Venue Settings",				/* name */
		CISCO_ELEM_AP_VENUE_SETTINGS,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		5,1024,					/* min/max length */
		cisco_ap_venue_settings,				/* type */
		"cisco/ap-venue-settings",			/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	}
	,



	{ 
		"Rouge Detection",				/* name */
		CISCO_LWELEM_ROUGE_DETECTION,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		7,7,					/* min/max length */
		cisco_rouge_detections,				/* type */
		"cisco/rouge-detection",				/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	}
	,

	{ 
		"Add Cisco WLAN",			/* name */
		CISCO_ELEM_ADD_WLAN,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		7,1117,					/* min/max length */
		cisco_add_wlan,				/* type */
		"radio/wlan",				/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct,			/* put */
		cisoc_add_wlan_mkkey
	}
	,
	
	{ 
		"Add Cisco WLAN (LWAPP)",		/* name */
		CISCO_LWELEM_ADD_WLAN,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		7,1117,					/* min/max length */
		cisco_add_lwwlan,			/* type */
		"radio/wlan",				/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct,			/* put */
		cisoc_add_lwwlan_mkkey
	}
	,


	{ 
		"SSC Hash Validation",			/* name */
		CISCO_LWELEM_SSC_HASH_VALIDATION,	/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_BOOL,				/* type */
		"cisco/ssh-hash-validation",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	}
	,

	{ 
		"SSC Hash",					/* name */
		CISCO_LWELEM_SSC_HASH,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		1,331,					/* min/max length */
		cisco_ssc_hash,				/* type */
		"cisco/hash",				/* Key */
		cw_in_generic_struct,				/* get */
		cw_out_generic_struct				/* put */
	}
	,
	{ 
		"Delete WLAN (Cisco LWAPP)",		/* name */
		CISCO_LWELEM_DELETE_WLAN,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		4,4,					/* min/max length */
		NULL,					/* type */
		"cisco-del-wlan",			/* Key */
		cisco_in_lw_del_wlan,			/* get */
		NULL,					/* put */
		NULL
	}
	,
	
	
	{ 
		"Radio Operational State (Draft 7)",	/* name */
		CAPWAP_ELEM_RADIO_OPERATIONAL_STATE,	/* Element ID */
		0,0,					/* Vendor / Proto */
		3,3,					/* min/max length */
		cisco_radio_oper_state,			/* type */
		"operational-state",			/* Key */
		cw_in_radio_generic_struct,		/* get */
		cw_out_radio_generic_struct,		/* put */
		NULL					/* mkkey */
	}
	,

	{ 
		"Hardware Info",			/* name */
		CISCO_LWELEM_HARDWARE_INFO,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		17,417,					/* min/max length */
		cisco_hardware_info,			/* type */
		"cisco/hardware-info",			/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	},

	{ 
		"Cisco Discovery Protocol",		/* name */
		CISCO_LWELEM_DISCOVERY_PROTOCOL,	/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		3,3,					/* min/max length */
		cisco_discovery_protocol,		/* type */
		"cisco/cisco-discovery-protocol",	/* Key */
		cw_in_generic_struct,			/* get */
		cw_out_generic_struct			/* put */
	},

	{ 
		"RAD Extended Config",			/* name */
		CISCO_LWELEM_RAD_EXTENDED_CONFIG,	/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		13,13,					/* min/max length */
		cisco_rad_extended_config,		/* type */
		"cisco/rad-extended-config",		/* Key */
		cw_in_radio_generic_struct,			/* get */
		cw_out_radio_generic_struct			/* put */
	},

	{
		"CAPWAP Timers",			/* name */
		CISCO_ELEM_CAPWAP_TIMERS,		/* Element ID */
		0, 0,						/* Vendor / Proto */
		2, 2,						/* min/max length */
		cisco_capwap_timers,					/* type */
		"capwap-timers",					/* Key */
		cw_in_generic_struct,				/* get */
		cw_out_generic_struct				/* put */
	}
	,

	{
		"8021.11 Assoc Limit (Cisco)",			/* name */
		CISCO_ELEM_80211_ASSOC_LIMIT,			/* Element ID */
		CW_VENDOR_ID_CISCO, 0,						/* Vendor / Proto */
		4, 4,						/* min/max length */
		cisco_80211_assoc_limit,			/* type */
		"cisco-8011-assoc-limit",			/* Key */
		cw_in_generic_struct,				/* get */
		cw_out_generic_struct				/* put */
	}
	,

	{
		"CISCO Sig Toggle",				/* name */
		CISCO_ELEM_SIG_TOGGLE,				/* Element ID */
		CW_VENDOR_ID_CISCO, 0,				/* Vendor / Proto */
		1, 1,						/* min/max length */
		CW_TYPE_BOOL,					/* type */
		"cisco/sig-toogle",				/* Key */
		cw_in_generic,					/* get */
		cw_out_generic					/* put */
	}
	,

	{
		"Dot11r WLC Mac And IP (Cisco)",		/* name */
		CISCO_LWELEM_DOT11R_WLC_MAC_AND_IP,		/* Element ID */
		0, 0,						/* Vendor / Proto */
		4, 4,						/* min/max length */
		cisco_dot11r_wlc_mac_and_ip,			/* type */
		"cisco/dot11r-wlc-mac-and-ip",			/* Key */
		cw_in_generic_struct,				/* get */
		cw_out_generic_struct				/* put */
	}
	,
	
	{
		"AP DTLS Data CFG",				/* name */
		CISCO_LWELEM_AP_DTLS_DATA_CFG,			/* Element ID */
		CW_VENDOR_ID_CISCO, CW_PROTO_LWAPP,		/* Vendor / Proto */
		3, 3,						/* min/max length */
		CW_TYPE_BSTR16,					/* type */
		"cisco/ap-dtls-data-cfg",			/* Key */
		cw_in_generic,					/* get */
		cw_out_generic					/* put */
	}
	,
	{
		"Operational State Detail Cause",		/* name */
		CISCO_ELEM_OPER_STATE_DETAIL_CAUSE,		/* Element ID */
		CW_VENDOR_ID_CISCO, 0,				/* Vendor / Proto */
		4, 4,						/* min/max length */
		CW_TYPE_WORD,					/* type */
		"cisco/operational-state-detail-cause",		/* Key */
		cw_in_radio_generic,				/* get */
		cw_out_radio_generic				/* put */
	}
	,
	{0,0,0,0,0,0,0,0}

};


/*static uint16_t discovery_request_states[] = {CAPWAP_STATE_DISCOVERY,0};*/
static struct cw_ElemDef discovery_request_elements[] ={
/*	{0,0,			CAPWAP_ELEM_WTP_DESCRIPTOR,	1, 0},*/
	{0,0,			CAPWAP_ELEM_WTP_BOARD_DATA,	0, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_RAD_NAME,		1, 0},
	{0,CW_VENDOR_ID_CISCO,	CW_CISCO_BOARD_DATA_OPTIONS,	0, 0},
	{0,0,0,00}
	
};

/*static uint16_t discovery_response_states[] = {CAPWAP_STATE_DISCOVERY,0};*/
static struct cw_ElemDef discovery_response_elements[] ={
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_TIMESYNC,			1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_MWAR_TYPE,			0, 0},
	{0,0,			CAPWAP80211_ELEM_WTP_RADIO_INFORMATION,	0, 0},
	{0,0,0,00}
	
};

/*static uint16_t join_request_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef join_request_elements[] ={
	{0,CW_VENDOR_ID_CISCO,	CW_CISCO_BOARD_DATA_OPTIONS,	1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_GROUP_NAME,	1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_MWAR_ADDR,		1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PATH_MTU,	0, 0},

	
	{0,0,			CAPWAP_ELEM_WTP_IPV4_IP_ADDRESS,1,0},
	{0,0,			CAPWAP_ELEM_WTP_IPV6_IP_ADDRESS,1,0},

	{0,0,CAPWAP_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS,		0, CW_DELETE},
	{0,0,CAPWAP_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS,		0, CW_DELETE},
	{0,0,			CAPWAP_ELEM_ECN_SUPPORT,	0, CW_DELETE},


	{0,0,0,00}
	
};

/*static uint16_t join_response_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef join_response_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PATH_MTU,	0, 0},

	{0,0,			CAPWAP_ELEM_ECN_SUPPORT,	0, CW_DELETE},

	{0,0,0,00}
	
};


/*static uint16_t configuration_status_request_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef configuration_status_request_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},


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
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AC_NAME_WITH_INDEX,		0, CW_IGNORE},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_CORE_DUMP,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_VENUE_SETTINGS,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_MAC_OPERATION,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_TX_POWER,			0, 0},	
	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_USERNAME_PASSWORD,	1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_LOGHOST_CONFIG,		1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_TELNET_SSH,		1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_SUBMODE,		1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_ETHERNET_PORT_SUBTYPE,	1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AC_IP_ADDR_WITH_INDEX,	0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_VLAN,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_TCP_ADJUST_MSS,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_HARDWARE_INFO,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_ROUGE_DETECTION,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_DTLS_DATA_CFG,			0, 0},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_DISCOVERY_PROTOCOL,	1, 0},
	{0,0,0,00}
	
};

/*static uint16_t configuration_status_response_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef configuration_status_response_elements[] ={
	{0,0,			CAPWAP_ELEM_RADIO_ADMINISTRATIVE_STATE,	1,0},
	{0,0,			CAPWAP_ELEM_RADIO_OPERATIONAL_STATE,	1,0},
	
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},
	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_MULTI_DOMAIN_CAPABILITY,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_WTP_RADIO_CONFIGURATION,	1, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_LED_STATE_CONFIG,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_LOG_FACILITY,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_MAC_OPERATION,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_TX_POWER,			0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_QOS,			0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AIRSPACE_CAPABILITY,		0, 0},
	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_MWAR_HASH_VALUE,	1, 0},

	{0,0,0,0}
};


/*static uint16_t configuration_update_request_states[] = {CAPWAP_STATE_RUN,0};*/
static struct cw_ElemDef configuration_update_request_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},
	
	{0,0,			CAPWAP_ELEM_RADIO_OPERATIONAL_STATE,	0,0},
	
	
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
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_REGULATORY_DOMAIN,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_MODEL,			0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_RESET_BUTTON_STATE,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_WTP_RADIO_CONFIGURATION,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_LOCATION_DATA,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_RAD_NAME,			0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_CORE_DUMP,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_STATISTICS_TIMER,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AC_NAME_WITH_INDEX,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_VENUE_SETTINGS,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_80211_ASSOC_LIMIT,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SIG_TOGGLE,			0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_MAC_OPERATION,		0, 0},

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_ADD_WLAN,			0, CW_IGNORE},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_USERNAME_PASSWORD,	0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_LOGHOST_CONFIG,		0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_TELNET_SSH,		0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_SUBMODE,		0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_ETHERNET_PORT_SUBTYPE,	0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AC_IP_ADDR_WITH_INDEX,	0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_FAILOVER_PRIORITY,	0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_VLAN,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_SSC_HASH_VALIDATION,	0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_SSC_HASH,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_DISCOVERY_PROTOCOL,	0, 0},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_ADD_WLAN,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_DELETE_WLAN,		0, 0},
	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_TCP_ADJUST_MSS,		0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_ROUGE_DETECTION,		0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_RAD_EXTENDED_CONFIG,	0, 0},
	
	{0,0,0,00}
	
};

/*static uint16_t wtp_event_request_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef wtp_event_request_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_ADD_WLAN,			0, CW_IGNORE},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_ADD_WLAN,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_HARDWARE_INFO,			0, 0},

	{0,0,0,0,0}
};

/*static uint16_t wtp_event_response_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef wtp_event_response_elements[] ={
/*	{0,0,CAPWAP_ELEM_RESULT_CODE,				1, 0},*/

	{0,0,0,0,0}
};


static struct cw_ElemDef change_state_event_request_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,	0, CW_IGNORE},
	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_ADD_WLAN,			0, CW_IGNORE},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_OPER_STATE_DETAIL_CAUSE,	0, CW_IGNORE},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_HARDWARE_INFO,			0, 0},

	{0,0,0,0,0}
};

/*static uint16_t wtp_event_response_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef change_state_event_response_elements[] ={
/*	{0,0,CAPWAP_ELEM_RESULT_CODE,				1, 0},*/

	{0,0,0,0,0}
};





/*static uint16_t wtp_echo_response_states[] = {CAPWAP_STATE_RUN,0};*/
static struct cw_ElemDef wtp_echo_response_elements[] ={
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_TIMESYNC,			1, 0},
	{0,0,0,0,0}
};


static struct cw_MsgDef messages[] = {
	{
		NULL,				/* name */
		CAPWAP_MSG_DISCOVERY_REQUEST,	/* type */
		CW_ROLE_AC,			/* role */
		NULL,				/* states */
		discovery_request_elements,	/* elements */
		NULL,				/* preprocess fun */
		postprocess_discovery		/* postprocess fun */
	},
	{
		NULL,				/* name */
		CAPWAP_MSG_DISCOVERY_RESPONSE,	/* type */
		CW_ROLE_WTP,
		NULL,
		discovery_response_elements,
		
	},
	{
		NULL,				/* name */
		CAPWAP_MSG_JOIN_REQUEST,	/* type */
		CW_ROLE_AC,			/* role to receive */
		NULL,				/* states, taken from mod_capwap*/
		join_request_elements,
		preprocess_join_request,	/* preprocess fun */
		postprocess_join_request	/* postprocess */
	},
	{
		NULL,				/* name */
		CAPWAP_MSG_JOIN_RESPONSE,	/* type */
		CW_ROLE_WTP,
		NULL,
		join_response_elements
	},


	{
		NULL,						/* name */
		CAPWAP_MSG_CONFIGURATION_STATUS_REQUEST,	/* type */
		CW_ROLE_AC,
		NULL,						/* states */
		configuration_status_request_elements,
		NULL					/* postprocess */
	},

	{
		NULL,						/* name */
		CAPWAP_MSG_CONFIGURATION_STATUS_RESPONSE,	/* type */
		CW_ROLE_WTP,
		NULL,
		configuration_status_response_elements,
		NULL					/* postprocess */
	},

	{
		NULL,						/* name */
		CAPWAP_MSG_CONFIGURATION_UPDATE_REQUEST,	/* type */
		CW_ROLE_WTP,
		NULL,
		configuration_update_request_elements,
		NULL					/* postprocess */
	},


	{
		NULL,					/* name */
		CAPWAP_MSG_WTP_EVENT_REQUEST,		/* msg type */
		CW_ROLE_AC,					/* role */
		NULL,				/* allowed states */
		wtp_event_request_elements		/* msg elements */
	},

	{
		NULL,					/* name */
		CAPWAP_MSG_WTP_EVENT_RESPONSE,		/* msg type */
		CW_ROLE_WTP,					/* role */
		NULL,		/* allowed states */
		wtp_event_response_elements		/* msg elements */
	},

	{
		NULL,					/* name */
		CAPWAP_MSG_CHANGE_STATE_EVENT_REQUEST,	/* msg type */
		CW_ROLE_AC,					/* role */
		NULL,					/* allowed states */
		change_state_event_request_elements		/* msg elements */
	},

	{
		NULL,						/* name */
		CAPWAP_MSG_CHANGE_STATE_EVENT_RESPONSE,		/* msg type */
		CW_ROLE_WTP,					/* role */
		NULL,		/* allowed states */
		change_state_event_response_elements		/* msg elements */
	},



	{
		NULL,					/* name */
		CAPWAP_MSG_ECHO_RESPONSE,		/* msg type */
		CW_ROLE_WTP,					/* role */
		NULL,		/* allowed states */
		wtp_echo_response_elements		/* msg elements */
	},

	{0,0,0,0}

};


static struct cw_ElemHandler handlers75[] = {
	{ 
		"WTP Radio Configuration  (Version >= 7.5)",/* name */
		CISCO_ELEM_WTP_RADIO_CONFIGURATION,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		28,28,					/* min/max length */
		cisco_wtp_radio_config75,		/* type */
		"cisco/wtp-radio-config",		/* Key */
		cw_in_radio_generic_struct,		/* get */
		cw_out_radio_generic_struct		/* put */
	}
	,
	{ 
		"Mac Operation (Version >= 7.5)",	/* name */
		CISCO_ELEM_MAC_OPERATION,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		17,17,					/* min/max length */
		cisco_mac_operation75,			/* type */
		"cisco/mac-operation",			/* Key */
		cw_in_radio_generic_struct,		/* get */
		cw_out_radio_generic_struct		/* put */
	},

	{0,0,0,0,0,0,0,0}
};






static struct cw_MsgDef messages75[] = {
	{0,0,0,0}
};



static cw_StateMachineState_t statemachine_states[]={
	{
		CAPWAP_STATE_CONFIGURE,CAPWAP_STATE_DATA_CHECK,
		NULL,0,
		1, NULL,
		CAPWAP_STATE_DATA_CHECK,CAPWAP_STATE_RUN
	}
	,
	{0,0,0}
};


static int (*postprocess_join_request_parent)(struct conn * conn);
struct cw_MsgSet * cisco_register_msg_set(struct cw_MsgSet * set, int mode){

	struct cw_MsgData * md;
        
	if (mode != CW_MOD_MODE_CAPWAP)
                return NULL;
		
	md = cw_msgset_get_msgdata(set,CAPWAP_MSG_JOIN_REQUEST);
	if (md != NULL){
		postprocess_join_request_parent = md->postprocess;
	}
	
        cw_msgset_add(set,messages, handlers73);
	cw_msgset_add_states(set,statemachine_states);
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
		cw_ktv_add(conn->local_cfg,"ac-descriptor/software/version",CW_TYPE_BSTR16, NULL,
			wtpver->type->data(wtpver),wtpver->type->len(wtpver));

		if(wtpver->type->len(wtpver)==4){
			uint32_t rv;
			rv = cw_get_dword(wtpver->type->data(wtpver));
			if (rv >= 0x07056600){
				cw_msgset_add(conn->msgset,messages75, handlers75);
			}
		}
	}
}

static int postprocess_discovery(struct conn *conn)
{
	if (conn->role == CW_ROLE_AC ){
		set_ac_version(conn);
		cw_detect_nat(conn);
	}
	return 1;
}

static int postprocess_join_request(struct conn *conn)
{
	if (postprocess_join_request_parent!=NULL){
		postprocess_join_request_parent(conn);
	}
	postprocess_discovery(conn);
	return 1;
}

static int preprocess_join_request(struct conn *conn)
{
	cw_KTV_t * ver;
	int use_ac_version;
	char verstr[512];

	if (conn->role != CW_ROLE_WTP)
		return 0;
		
	use_ac_version = cw_ktv_get_bool(conn->local_cfg,"cisco/wtp-use-ac-version",0);

	if (use_ac_version){
		ver = cw_ktv_get(conn->remote_cfg,"ac-descriptor/software/version", CW_TYPE_BSTR16);
		cw_ktv_replace(conn->local_cfg,"wtp-descriptor/software/version",CW_TYPE_BSTR16, NULL,
			ver->type->data(ver),ver->type->len(ver));
			
		cw_format_version(verstr,ver->type->data(ver),ver->type->len(ver));
		cw_dbg(DBG_INFO, "Cisco WTP - Using AC's software version: %s", verstr);
		
	}
	else{
		ver = cw_ktv_get(conn->local_cfg,"wtp-descriptor/software/version", CW_TYPE_BSTR16);
		cw_format_version(verstr,ver->type->data(ver),ver->type->len(ver));
		cw_dbg(DBG_INFO, "Cisco - WTP Using own software version: %s", verstr);
	}
	
	if(ver->type->len(ver)==4){
		uint32_t rv;
		rv = cw_get_dword(ver->type->data(ver));
		if (rv >= 0x07056600){
			cw_msgset_add(conn->msgset,messages75, handlers75);
		}
	}
	return 1;
}