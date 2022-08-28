/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    actube is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "cw/capwap.h"
#include "cw/capwap80211.h"
#include "cw/msgset.h"
#include "cw/val.h"
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

int cisco_out_radio_info(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst);


static cw_ValValRange_t cfg_type[]={
	{1,1,"1 - global"},
	{2,2,"2 - custom"},
	{0,0,NULL}
};


static cw_ValStruct_t ap_time_sync[] = {
	{CW_TYPE_DWORD, "timestamp", 4,-1},
	{CW_TYPE_BYTE, "type", 1,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t mwar_addr[] = {
	{CW_TYPE_BYTE, "mwar-type", 1,-1},
	{CW_TYPE_IPADDRESS, "address", 4,-1},
	{CW_TYPE_WORD, "unknown", 2,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_lw_path_mtu[] = {
	{CW_TYPE_WORD, "max", 2,-1},
	{CW_TYPE_WORD, "len", 2,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_ap_uptime[] = {
	{CW_TYPE_DWORD, "current-uptime", 4,-1},
	{CW_TYPE_DWORD, "last-uptime", 4,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_login[] = {
	{CW_TYPE_STR,	"username",		33,	-1	},
	{CW_TYPE_STR,	"password",		121,	-1	},
	{CW_TYPE_STR,	"enable-password",	121,	33+121	},
	{CW_TYPE_WORD,	"option",		2,	275	},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_8021xlogin[] = {
	{CW_TYPE_STR,	"username",		33,	-1	},
	{CW_TYPE_STR,	"password",		121,	-1	},
	{CW_TYPE_WORD,	"option",		2,	275	},
	{NULL,NULL,0,0}
};





static cw_ValStruct_t cisco_elem_15[] = {
	{CW_TYPE_BYTE, "cfg-type", 1, -1, cfg_type},
	{CW_TYPE_BYTE, "channel", 1,-1},
	{CW_TYPE_BSTR16, "rest",-1,-1},
	{NULL,NULL,0,0}
};




static cw_ValEnum_t cisco_ap_username_and_password_enum[] ={
	{2,	"802.1x-credentials",	CW_TYPE_STRUCT, cw_in_generic, cw_out_generic, cisco_8021xlogin },

	{1,	"login-credentials",	CW_TYPE_STRUCT, cw_in_generic, cw_out_generic, cisco_login },

	{0,0,0,0}
};


static cw_ValIndexed_t cisco_ap_username_and_password = {
	276,cisco_ap_username_and_password_enum
};


static cw_ValStruct_t cisco_loghost_config[] = {
	{CW_TYPE_IPADDRESS,	"loghost",		4,	-1},
	{CW_TYPE_STR,		"last-joined-ap",	32,	-1},
	{NULL,NULL,0,0}
};


static cw_ValStruct_t cisco_ap_led_state_config70[] = {
	{CW_TYPE_BYTE,		"led-state",		1,	-1},
	{NULL,NULL,0,0}
};


static cw_ValStruct_t cisco_ap_led_state_config73[] = {
	{CW_TYPE_BYTE,		"led-state",		1,	-1},
	{CW_TYPE_BYTE,		"save-flag",		1,	-1},
	{NULL,NULL,0,0}
};

static cw_ValEnum_t cisco_ap_telnet_ssh_enum[] ={
	{0,	"telnet",	CW_TYPE_BOOL, cw_in_generic, cw_out_generic },
	{1,	"ssh",		CW_TYPE_BOOL, cw_in_generic, cw_out_generic },
	{0,0,0,0}
};

static cw_ValIndexed_t cisco_ap_telnet_ssh = {
	1,cisco_ap_telnet_ssh_enum
};

static cw_ValStruct_t cisco_multi_domain_capability[]={
	{CW_TYPE_BYTE,		"reserved",		1,	-1},
	{CW_TYPE_WORD,		"first-channel",	2,	-1},
	{CW_TYPE_WORD,		"number-of-channels",	2,	-1},
	{CW_TYPE_WORD,		"max-tx-power-level",	2,	-1},
	{NULL,NULL,0,0}
};




static cw_ValStruct_t cisco_wtp_board_data[]={
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

static cw_ValStruct_t cisco_wtp_board_data_options[]={
	{CW_TYPE_BYTE,		"ant-type",		1,	-1},
	{CW_TYPE_BYTE,		"flex-connect",		1,	-1},
	{CW_TYPE_BYTE,		"ap-type",		1,	-1},
	{CW_TYPE_BYTE,		"failover-priority",	1,	-1},

	{NULL,NULL,0,0}
};


static cw_ValStruct_t cisco_ap_led_flash_config[]={
	{CW_TYPE_BYTE,		"flash-enable",		1,	0},
	{CW_TYPE_DWORD,		"flash-sec",		4,	4},
	{CW_TYPE_BYTE,		"save-flag",		4,	8},

	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_ap_static_ip_addr[]={
	{CW_TYPE_IPADDRESS,"address",	4,-1},
	{CW_TYPE_IPADDRESS,"netmask",	4,-1},
	{CW_TYPE_IPADDRESS,"gateway",	4,-1},
	{CW_TYPE_BOOL,"enabled",	1,-1},
	{CW_TYPE_IPADDRESS,"unknown",	4,-1},
	{NULL,NULL,0,0}
};


static cw_ValStruct_t cisco_ap_static_dns[]={
	{CW_TYPE_BOOL,"enable",	1,-1},
	{CW_TYPE_IPADDRESS,"ip",	4,-1},
	{NULL,NULL,0,0}
};


static cw_ValStruct_t cisco_ap_static_domain[]={
	{CW_TYPE_BOOL,"enable",	1,-1},
	{CW_TYPE_BSTR16,"name",	-1,-1},
	{NULL,NULL,0,0}
};



static cw_ValStruct_t cisco_ap_regulatory_domain4[]={
	{CW_TYPE_BOOL,"set",1,-1},
	{CW_TYPE_BYTE,"slot",1,-1},
	{CW_TYPE_BYTE,"code0",1,-1},
	{CW_TYPE_BYTE,"code1",1,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_ap_regulatory_domain5[]={
	{CW_TYPE_BYTE,"band-id",1,-1},
	{CW_TYPE_BOOL,"set",1,-1},
	{CW_TYPE_BYTE,"slot",1,-1},
	{CW_TYPE_BYTE,"code0",1,-1},
	{CW_TYPE_BYTE,"code1",1,-1},
	{NULL,NULL,0,0}
};


static cw_ValStruct_t cisco_mac_operation70[]={
	{CW_TYPE_BYTE,"reserved",1,-1},
	{CW_TYPE_WORD,"rts-threshold",2,-1},
	{CW_TYPE_BYTE,"short-retry",1,-1},
	{CW_TYPE_BYTE,"long-retry",1,-1},
	{CW_TYPE_WORD,"fragmentation-threshold",2,-1},
	{CW_TYPE_DWORD,"tx-msdu-lifetime",4,-1},
	{CW_TYPE_DWORD,"rx-msdu-lifetime",4,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_mac_operation75[]={
	{CW_TYPE_WORD,"reserved",2,-1},
	{CW_TYPE_WORD,"rts-threshold",2,-1},
	{CW_TYPE_BYTE,"short-retry",1,-1},
	{CW_TYPE_BYTE,"long-retry",1,-1},
	{CW_TYPE_WORD,"fragmentation-threshold",2,-1},
	{CW_TYPE_DWORD,"tx-msdu-lifetime",4,-1},
	{CW_TYPE_DWORD,"rx-msdu-lifetime",4,-1},
	{NULL,NULL,0,0}
};


static cw_ValStruct_t cisco_ap_power_injector_config[]={
	{CW_TYPE_BYTE,"state",1,-1},
	{CW_TYPE_BYTE,"selection",1,-1},
	{CW_TYPE_BSTR16,"switch-mac-address",6,-1},
	{NULL,NULL,0,0}
};


int cisco_in_ap_regulatory_domain(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
	char key[CW_CFG_MAX_KEY_LEN];
	int idx;
	void * type;

	idx = cw_cfg_get_next_index(params->cfg,eh->key);

	sprintf(key,"%s.%d",eh->key,idx);
	
	if(len==4)
		type = cisco_ap_regulatory_domain4;
	if(len==5)
		type = cisco_ap_regulatory_domain5;
//	cw_ktv_read_struct(params->cfg,type,key,data,len);
	CW_TYPE_STRUCT->read(params->cfg,key,data,len,type);

	return 1;
}


int cisco_out_ap_regulatory_domain(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)

{
	char key[CW_CFG_MAX_KEY_LEN];
//	char testkey[CW_CFG_MAX_KEY_LEN];
	int idx;
	void * type;
	cw_Val_t * result;
	int len,start;
	uint8_t * ob;



	idx = 0;
	ob = dst;
	

	type = NULL;
	result = cw_cfg_get_val_l(params->cfg_list,"capwap/wtp-descriptor/software/version",CW_TYPE_BSTR16);
	if (result!=NULL){
		if(result->type->len(result)==4){
			uint32_t rv;
			rv = cw_get_dword(result->type->data(result));
cw_dbg(DBG_X,"Version is %08X",rv);			
//stop();
//			if (rv >= 0x07056600){
			if (rv > 0x07036500){
				type = cisco_ap_regulatory_domain5;
			}
			else{
				type = cisco_ap_regulatory_domain4;
			}
		}
		
	}
	else {
		stop();
	}

	cw_val_destroy(result);

	do {
		sprintf(key,"%s.%d",eh->key,idx);
		if (!cw_cfg_base_exists_l(params->cfg_list,key))
			break;
	
		if(type == NULL){
//			sprintf(testkey,"%s/%s",key,"band-id");
			stop();
//			result = cw_ktv_get_val_l(params->cfg_list,key,CW_TYPE_BYTE);
			if (result==NULL){
				type = cisco_ap_regulatory_domain4;
			}
			else{
				type = cisco_ap_regulatory_domain5;
				cw_val_destroy(result);
			}
		}
		
		start = params->msgset->header_len(eh);
		len = CW_TYPE_STRUCT->write(params->cfg_list,key,ob+start,type);
//		len = cw_ktv_write_struct(params->cfg,NULL,type,key,ob+start);
		ob += params->msgset->write_header(eh,ob,len);
		
		idx++;
		
	}while(1);


	sprintf(key,"%s.%d",eh->key,idx+1);
	return ob-dst;
}

static cw_ValStruct_t cisco_ap_model[]={
	{CW_TYPE_STR,"model",30,-1},
	{CW_TYPE_STR,"image",30,30},
	{NULL,NULL,0,0}
};



static cw_ValStruct_t cisco_direct_sequence_control70[]={
	{CW_TYPE_BYTE,"cfg-type",1,-1,cfg_type},
	{CW_TYPE_BYTE,"current-channel",1,-1},
	{CW_TYPE_BYTE,"current-cca-mode",1,-1},
	{CW_TYPE_DWORD,"energy-detect-threshold",4,-1},
	{CW_TYPE_BYTE,"unknown",1,-1},
	{NULL,NULL,0,0}
};


static int get_num_antennas70(cw_Cfg_t *cfg, 
		const char * key, const uint8_t *src, int len, const void *param, int *l)
{
	*l=0;
	return cw_get_byte(src-4);
};

static int put_num_antennas70(cw_Cfg_t *cfg, 
		const char * key, uint8_t *dst, const void *param, int l)
{
	return cw_put_byte((dst-4),l);
};

static cw_ValValRange_t antenna_type[]={
	{1,1,"1 - Internal Antenna"},
	{2,2,"2 - External Antenna"},
	{0,0,NULL}
};


struct cw_ValArrayDef ant_array = {
	get_num_antennas70,
	put_num_antennas70,
	CW_TYPE_BYTE,
	&antenna_type
};

static cw_ValStruct_t cisco_antenna_payload70[]={
	{CW_TYPE_BYTE,"diversity-selection",1,-1},
	{CW_TYPE_BYTE,"antenna-mode",1,-1},
	{CW_TYPE_BYTE,"antenna-cnt",1,-1},
	{CW_TYPE_BYTE,"unknown",1,-1},
	{CW_TYPE_BYTE,"802-11n-tx-antennas",1,-1},
	{CW_TYPE_BYTE,"802-11n-rx-antennas",1,-1},
	{CW_TYPE_ARRAY,"antenna",-1,-1,&ant_array},
	{NULL,NULL,0,0}
};

/*
static cw_ValStruct_t cisco_antenna_payload73[]={
	{CW_TYPE_BYTE,"diversity-selection",1,-1},
	{CW_TYPE_BYTE,"antenna-mode",1,-1},
	{CW_TYPE_BYTE,"number-of-antennas",1,-1},
	{CW_TYPE_BYTE,"unknown",1,-1},
	{CW_TYPE_BYTE,"802-11n-tx-antennas",1,-1},
	{CW_TYPE_BYTE,"802-11n-rx-antennas",1,-1},
//	{CW_TYPE_BYTE,"unknown",1,-1},
	{CW_TYPE_BYTE,"antenna-1",1,-1},
	{CW_TYPE_BYTE,"antenna-2",1,-1},
	{NULL,NULL,0,0}
};
*/






static cw_ValStruct_t cisco_wtp_radio_config70[]={
	{CW_TYPE_BYTE,"cfg-type",1,-1},
	{CW_TYPE_WORD,"occupancy-limit",2,-1},
	{CW_TYPE_BYTE,"cfg-period",1,-1},
	{CW_TYPE_WORD,"cfp-maximum-duration",2,-1},
	{CW_TYPE_BSTR16,"bssid",6,-1},
	{CW_TYPE_WORD,"beacon-period",2,-1},
	{CW_TYPE_STR,"country-str1",3,-1},
	{CW_TYPE_STR,"country-str2",3,-1},
/*	{CW_TYPE_BYTE,"gpr-period",1,-1},*/
	{CW_TYPE_DWORD,"reg",4,-1},
/*	{CW_TYPE_BYTE,"max-stations",1,-1},*/
	{NULL,NULL,0,0}
};



static cw_ValStruct_t cisco_wtp_radio_config73[]={
	{CW_TYPE_BYTE,"@cisco/cfg-type",1,-1,cfg_type},
	{CW_TYPE_WORD,"@cisco/occupancy-limit",2,-1},
	{CW_TYPE_BYTE,"@cisco/cfg-period",1,-1},
	{CW_TYPE_WORD,"@cisco/cfp-maximum-duration",2,-1},
	{CW_TYPE_BSTR16,"bssid",6,-1},
	{CW_TYPE_WORD,"beacon-period",2,-1},
	{CW_TYPE_STR,"@cisco/country-string",3,-1},
	{CW_TYPE_STR,"country-string",2,-1},
	{CW_TYPE_BSTR16,"country-string-attr",1,-1},
	{CW_TYPE_BYTE,"@cisco/gpr-period",1,-1},
	{CW_TYPE_DWORD,"@cisco/reg",4,-1},
	{CW_TYPE_BYTE,"@cisco/max-stations",1,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_wtp_radio_config75[]={
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




static cw_ValStruct_t cisco_tx_power[]={
	{CW_TYPE_BYTE,"@cisco/cfg-type",1,-1,cfg_type},
	{CW_TYPE_WORD,"current-tx-power",2,-1},
	{NULL,NULL,0,0}
};


static cw_ValStruct_t cisco_lw_radio_module_info_stru[]={
	{CW_TYPE_STR,"name",21,-1},
	{CW_TYPE_STR,"serial",11,-1},
	{CW_TYPE_STR,"type",25,-1},
	{CW_TYPE_STR,"descr",126,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_rrm_load_stru[]={
	{CW_TYPE_BYTE,"rx-load",1,-1},
	{CW_TYPE_BYTE,"tx-load",1,-1},
	{CW_TYPE_BYTE,"cca-load",1,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_performance_profile_stru[]={
	{CW_TYPE_BYTE,"cfg-type",1,-1,cfg_type},
	{CW_TYPE_WORD,"rf-busy-threshold",2,-1},
	{CW_TYPE_WORD,"num-clients-threshold",2,-1},
	{CW_TYPE_WORD,"bytes-per-sec",2,-1},
	{CW_TYPE_WORD,"foreigh-threshold",2,-1},
	{CW_TYPE_WORD,"rssi-threshold",2,-1},
	{CW_TYPE_WORD,"min-perf-snr",2,-1},
	{CW_TYPE_WORD,"excpection-level",2,-1},
	{CW_TYPE_WORD,"min-num-clients",2,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_phy_ht_cap_stru[]={
	{CW_TYPE_WORD,"ht-cap-info",2,-1},
	{CW_TYPE_WORD,"extendend-ht-cap-info",2,-1},
	{CW_TYPE_BSTR16,"rest",-1,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_phy_ht_control_stru[]={
	{CW_TYPE_BYTE,"enable-ht",1,-1},
	{CW_TYPE_BYTE,"cfg-type",1,-1,cfg_type},
	{CW_TYPE_BYTE,"current-freq",1,-1},
	{CW_TYPE_BSTR16,"rest",-1,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_station_cfg_stru[]={
	{CW_TYPE_BYTE,"cfg-type",1,-1,cfg_type},
	{CW_TYPE_BYTE,"current-freq",1,-1},
	{CW_TYPE_BSTR16,"rest",-1,-1},
	{NULL,NULL,0,0}
};



static cw_ValStruct_t cisco_ap_qos[]={
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

static cw_ValStruct_t cisco_ap_core_dump[]={
	{CW_TYPE_IPADDRESS,"tftp-server",4,-1},
	{CW_TYPE_BOOL,"compression",1,16},
	{CW_TYPE_STR,"filename",199,17},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_vlan[]={
	{CW_TYPE_BOOL,"tagging",1,-1},
	{CW_TYPE_WORD,"id",2,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_rouge_and_mss[]={
	{CW_TYPE_BOOL,"enable",1,-1},
	{CW_TYPE_WORD,"tcp-adjust-mss",2,-1},
	{NULL,NULL,0,0}
};


static cw_ValStruct_t cisco_rouge_detection70[]={
	{CW_TYPE_BOOL,"rouge-detection",1,-1},
	{CW_TYPE_BSTR16,"rest",4,-1},
	{NULL,NULL,0,0}
};



static cw_ValStruct_t cisco_rouge_detections[]={
	{CW_TYPE_BOOL,"rouge-detection",1,-1},
	{CW_TYPE_BSTR16,"rest",6,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_ap_venue_settings[]={
	{CW_TYPE_WORD,"group",2,-1},
	{CW_TYPE_BYTE,"type",1,-1},
	{CW_TYPE_STR,"language",3,-1},
	{CW_TYPE_STR,"name",-1,7},
	{NULL,NULL,0,0}
	
};

static cw_ValStruct_t cisco_ap_mode_and_type[]={
	{CW_TYPE_BYTE,"mode",1,-1},
	{CW_TYPE_BYTE,"type",1,-1},
	{NULL,NULL,0,0}
};


static cw_ValStruct_t dtls_data_cfg[]={
	{CW_TYPE_BOOL,"cabable",1,-1},
	{CW_TYPE_BOOL,"enabled",1,-1},
	{NULL,NULL,0,0}
};




/*
static cw_ValStruct_t cisco_add_wlan[]={
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
*/

static cw_ValStruct_t cisco_add_wlan70[]={
	{CW_TYPE_BYTE,"radio-id",1,-1},
	{CW_TYPE_WORD,"wlan-capability",2,-1},
	{CW_TYPE_BYTE,"wlan-id",1,4},
	{CW_TYPE_DWORD,"encryption-policy",4,5},

	{CW_TYPE_BSTR16,"wep-key",13,9},
	{CW_TYPE_BYTE,"wep-key-index",1,41},
	{CW_TYPE_BOOL,"wep-encryption",1,42},

	{CW_TYPE_BYTE,"qos",1,324},
	{CW_TYPE_WORD,"scan-defer-period",1,328},
	{CW_TYPE_WORD,"scan-defer-time",1,330},
	{CW_TYPE_BOOL,"broadcast-ssid",1,332},
	{CW_TYPE_BOOL,"aironet-ie",1,333},
	{CW_TYPE_BYTE,"hreap-local-switch",1,378},
	{CW_TYPE_WORD,"session-timout",2,381},

	{CW_TYPE_BYTE, "dtim-period",1,440},
	{CW_TYPE_STR,"profile-name",30,441},
	{CW_TYPE_STR, "ssid",33,474},

	
	{NULL,NULL,0,0}
};





static int cisco_in_lw_del_wlan(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
	stop();
/*
	int wlan_id, radio_id;
	char key[CW_CFG_MAX_KEY_LEN];
	
	radio_id=cw_get_byte(data);
	wlan_id=cw_get_word(data+1);
	sprintf(key,"radio.%d/wlan.%d",radio_id,wlan_id);
	cw_ktv_del_sub(params->cfg,key);
	cw_dbg(DBG_INFO,"Del WLAN rid=%d, id=%d",wlan_id);
	return 0;*/
}


/*
static int cw_mkradiokey(const char *pkey, uint8_t*data, int len, char *dst)
{
	int radio_id;
	radio_id = cw_get_byte(data);
	sprintf(dst,"radio.%d/%s",radio_id,pkey);
	return 1;
}*/

/*
static int cisoc_add_wlan_mkkey(const char *pkey, uint8_t*data, int len, char *dst)
{
	int wlan_id,radio_id;
	stop();	
	radio_id = cw_get_byte(data);
	wlan_id = cw_get_byte(data+3);
	sprintf(dst,"radio.%d/wlan.%d/add-wlan",radio_id,wlan_id);
	return 1;
}
*/


static int cisco_add_wlan_mkkey70(const char *pkey, uint8_t*data, int len, char *dst)
{
        int wlan_id,radio_id;
        radio_id = cw_get_byte(data);
        wlan_id = cw_get_byte(data+4);
        sprintf(dst,"radio.%d/wlan.%d/add-wlan",radio_id,wlan_id);
        return 1;
}


/*
static int cisco_patch_add_wlan70(uint8_t * data, void * st)
{
	stop();
	int * stack = st;
	cw_set_byte(data,stack[1]);
	cw_set_byte(data+3, stack[2]);
	return 0;
}

*/

static cw_ValStruct_t cisco_add_lwwlan[]={
	{CW_TYPE_BSTR16, "misc", 8, 2},
	{CW_TYPE_STR, "ssid",-1,10},
	{CW_TYPE_WORD, "misc2", 2, 48 },
	{NULL,NULL,0,0}
};

static cw_ValValRange_t oper_val_state[]={
	{1,1,"disabled"},
	{2,2,"enabled"},
	{0,0,NULL}
};

static cw_ValValRange_t oper_val_cause[]={
	{0,0,"Normal"},
	{1,1,"Radio Failure"},
	{2,2,"Software Failure"},
	{3,3,"Administratively Set"},
	{0,0,NULL}
};

static cw_ValStruct_t cisco_radio_oper_state[]={
	{CW_TYPE_BYTE, "state", 1, -1, oper_val_state},
	{CW_TYPE_BYTE, "cause", 1, -1, oper_val_cause},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_capwap_timers[] = {
	{CW_TYPE_BYTE, "max-discovery-interval", 1,-1},
	{CW_TYPE_BYTE, "echo-interval", 1,-1},
	{NULL,NULL,0,0}
};




static int cisoc_add_lwwlan_mkkey(const char *pkey, uint8_t*data, int len, char *dst)
{
	int wlan_id,radio_id;
	stop();	
	radio_id = cw_get_byte(data);
	wlan_id = cw_get_byte(data+1);
	sprintf(dst,"radio.%d/wlan.%d/add-lw-wlan",radio_id,wlan_id);
	return 1;
}

static int cisco_patch_add_lwwlan(uint8_t * data, void * st)
{
	stop();
	int * stack = st;
	cw_set_byte(data,stack[1]);
	cw_set_byte(data+1, stack[2]);
	return 0;
}



static cw_ValStruct_t cisco_ssc_hash[]={
	{CW_TYPE_BOOL,"validate",1,-1},
	{CW_TYPE_BSTR16,"hash",-1,-1},
	{NULL,NULL,0,0}
};



static cw_ValStruct_t cisco_hardware_info[]={
	{CW_TYPE_STR, "ram-type",20,-1},
	{CW_TYPE_STR, "cpu",40,40},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_discovery_protocol[]={
	{CW_TYPE_WORD, "data",2,-1},
	{CW_TYPE_BOOL, "enabled",1,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_rad_extended_config[]={
	{CW_TYPE_WORD, "beacon-interval",2,-1},
	{CW_TYPE_WORD, "beacon-range",2,-1},
	{CW_TYPE_WORD, "multicast-buffer",2,-1},
	{CW_TYPE_WORD, "multicast-data-range",2,-1},
	{CW_TYPE_WORD, "rx-sensop-threshold",2,-1},
	{CW_TYPE_WORD, "c-ccat",2,-1},
	{NULL,NULL,0,0}
};


int cw_out_traverse(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst);

static cw_ValStruct_t cisco_80211_assoc_limit[]={
	{CW_TYPE_BOOL, "enable",1,-1},
	{CW_TYPE_BYTE, "limit",1,-1},
	{CW_TYPE_WORD, "interval",1,-1},
	{NULL,NULL,0,0}
};

static cw_ValStruct_t cisco_dot11r_wlc_mac_and_ip[]={
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
	char key[CW_CFG_MAX_KEY_LEN];
	
	radio_id=cw_get_byte(data);
	wlan_id=cw_get_word(data+1);
	sprintf(key,"radio.%d/wlan.%d",radio_id,wlan_id);

	read_struct(eh->type,key);

	cw_dbg(DBG_INFO,"Del WLAN rid=%d, id=%d",wlan_id);
	return 0;
}
*/

static struct cw_ElemHandler handlers70[] = {
	
	{ 
		"AC Name - (zero-length allowed)",	/* name */
		CAPWAP_ELEM_AC_NAME,			/* Element ID */
		0,0,					/* Vendor / Proto */
		0,CAPWAP_MAX_AC_NAME_LEN,		/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"capwap/ac-name",			/* Key */
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
		"capwap/wtp-descriptor",	/* Key */
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
		"capwap/ac-descriptor",		/* Key */
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
		"capwap/wtp-name",		/* Key */
		cw_in_generic,			/* get */
		cw_out_generic			/* put */
	}
	,
	{
		"Board Data Options",		/* name */
		CW_CISCO_BOARD_DATA_OPTIONS,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,		/* Vendor / Proto */
		4,4,				/* min/max length */
		CW_TYPE_STRUCT,			/* type */
		"cisco/wtp-board-data/options",	/* Key */
		cw_in_generic,			/* handler */
		cw_out_generic,			/* put */
		NULL,
		NULL,
		cisco_wtp_board_data_options,	/* struct datae */
	}
	,
	{
		"AP Time Sync",			/* name */
		CISCO_ELEM_AP_TIMESYNC,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,		/* Vendor / Proto */
		5,5,				/* min/max length */
		CW_TYPE_STRUCT,			/* type */
		"cisco/ap-timesync",		/* Key */
		cw_in_generic,			/* handler */
		cisco_out_ap_timesync,		/* put */
		NULL,
		NULL,
		ap_time_sync

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
		"capwap80211/wtp-radio-information",		/* Key */
		cw_in_radio_generic,				/* get */
		cisco_out_radio_info				/* put */ 
	}
	,
	{ 
		"Session ID (Cisco min len = 4)",	/* name */
		CAPWAP_ELEM_SESSION_ID,			/* Element ID */
		0,0,					/* Vendor / Proto */
		4,CAPWAP_SESSION_ID_LEN,		/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"capwap/session-id",			/* Key */
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
		CW_TYPE_STRUCT,				/* type */
		"cisco/mwar-addr",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		mwar_addr
	}
	,
	{ 
		"WTP IPv4 Address (Draft 7)",		/* name */
		CAPWAP_ELEM_WTP_IPV4_IP_ADDRESS,	/* Element ID */
		0,0,					/* Vendor / Proto */
		4,4,					/* min/max length */
		CW_TYPE_IPADDRESS,			/* type */
		"capwap/local-ip-address",		/* Key */
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
		"capwap/local-ip-address",		/* Key */
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
		NULL,					/* put */
		NULL,
		NULL,
		NULL,					/* param */
		1
	}
	,
	{ 
		"Cisco LWAPP Path MTU",			/* name */
		CISCO_LWELEM_PATH_MTU,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		0,0,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/lw-path-mtu",			/* Key */
		cw_in_generic,				/* get */
		cisco_out_lw_path_mtu,			/* put */
		NULL,
		NULL,
		cisco_lw_path_mtu,
	}
	,
	
	{ 
		"AP Uptime",				/* name */
		CISCO_ELEM_AP_UPTIME,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		8,8,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-uptime",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_uptime,

	}
	,
	
	{ 
		"AP Username and Password",		/* name */
		CISCO_LWELEM_AP_USERNAME_PASSWORD,	/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		0,0,					/* min/max length */
		&cisco_ap_username_and_password,	/* type */
		"cisco/ap-username-and-password",	/* Key */
		cw_in_generic_indexed_enum,		/* get */
		cw_out_generic_indexed_enum,		/* put */
		NULL,
		NULL,
	}
	,
	{ 
		"Loghost Config",			/* name */
		CISCO_LWELEM_AP_LOGHOST_CONFIG,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		36,36,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/loghost-config",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_loghost_config,			/* type */

	}
	,
	{ 
		"AP LED State Config (v7.0)",			/* name */
		CISCO_ELEM_AP_LED_STATE_CONFIG,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-led-state-config",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_led_state_config70

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
		CW_TYPE_STRUCT,				/* type */
		"cisco/multi-domain-capability",	/* Key */
		cw_in_radio_generic,				/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_multi_domain_capability,
			
	}
	,
	
	{ 
		"Cisco WTP Board Data",			/* name */
		CISCO_ELEM_WTP_BOARD_DATA,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		8,48,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/wtp-board-data",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_wtp_board_data,
	},
	{ 
		"AP LED Flash Config",			/* name */
		CISCO_ELEM_AP_LED_FLASH_CONFIG,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		0,48,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-led-flash-config",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_led_flash_config

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
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-power-injector-config",	/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_power_injector_config

	},

	{ 
		"AP Mode And Type",			/* name */
		CISCO_ELEM_AP_MODE_AND_TYPE,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		2,2,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-mode-and-type",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_mode_and_type,

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
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-static-ip-addr",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_static_ip_addr,

	},
	{ 
		"AP Static DNS",			/* name */
		CISCO_ELEM_AP_DNS,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		5,5,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-static-dns",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_static_dns,

	},

	{ 
		"AP Static Domain",			/* name */
		CISCO_ELEM_AP_DOMAIN,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,128,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-static-domain",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_static_domain,

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
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-model",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_model,
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

	{ /* WTP Radio Configuration for AC/WPT with version 7.0 */
	
		"WTP Radio Configuration (v7.0)",		/* name */
		CISCO_ELEM_WTP_RADIO_CONFIGURATION,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		25,25,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"capwap80211/wtp-radio-config",		/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_wtp_radio_config70,

	}
	,

	{ /* WTP Direct Sequence Control for AC/WPT with version 7.0 */
	
		"Direct Sequence Control (v7.0)",	/* name */
		CISCO_ELEM_DIRECT_SEQUENCE_CONTROL,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		9,9,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/direct-sequence-control",	/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,					/* mkkey */
		NULL,
		cisco_direct_sequence_control70,	/* param */

	}
	,

	{ /* WTP Antenna Payload AC/WPT with version 7.0 */
	
		"Antenna Payload (v7.0)",		/* name */
		CW_CISCO_ANTENNA_PAYLOAD,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		9,100,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/antenna-payload",		/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,					/* mkkey */
		NULL,
		cisco_antenna_payload70,


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
		"Mac Operation (7.0)",			/* name */
		CISCO_ELEM_MAC_OPERATION,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		16,16,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/mac-operation",			/* Key */
		cw_in_radio_generic,				/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_mac_operation70,		


	},

	{ 
		"Tx Power",				/* name */
		CISCO_ELEM_TX_POWER,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		4,4,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"capwap80211/tx-power",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_tx_power,

	},

	{ 
		"Tx Power Levels",			/* name */
		CISCO_ELEM_TX_POWER_LEVELS,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		5,150,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/tx-power-levels",		/* Key */
		cw_in_radio_generic,				/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,


	},

	{ 
		"Channel Power",			/* name */
		CISCO_ELEM_CHANNEL_POWER,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		5,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/channel-power",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},


	{ 
		"Spam Domain Secret",			/* name */
		CISCO_ELEM_SPAM_DOMAIN_SECRET,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/spam-domain-secret",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},


	{ 
		"Cisco Elem 132",			/* name */
		CISCO_ELEM_132,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem132",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ 
		"Cisco Elem 33",			/* name */
		CISCO_ELEM_33,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem33",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ 
		"Cisco Elem 15 - Channel Setting (?)",	/* name */
		CISCO_ELEM_15,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/channel-setting",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_elem_15

	},

	{ 
		"Cisco - Supported Rates",		/* name */
		CISCO_ELEM_SUPPORTED_RATES,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem16",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		NULL,

	},

	{ 
		"Cisco Elem 25 ",		/* name */
		CISCO_ELEM_RRM_LOAD,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		4,4,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/rrm",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_rrm_load_stru,

	},




	{ 
		"Cisco Elem 39",			/* name */
		CISCO_ELEM_39,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem39",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco Elem 145",			/* name */
		CISCO_ELEM_145,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem145",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco Elem 146",			/* name */
		CISCO_ELEM_146,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem146",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco Elem 153",			/* name */
		CISCO_ELEM_153,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem153",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco Elem 156",			/* name */
		CISCO_ELEM_156,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem156",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco - Spam CFP Status",		/* name */
		CISCO_ELEM_SPAM_CFP_STATUS,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		2,2,					/* min/max length */
		CW_TYPE_BOOL,				/* type */
		"cisco/spam-cfp-status",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},




	{ 
		"Cisco LWAPP Elem 9",			/* name */

		CISCO_LWELEM_9,				/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/lwelem9",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco LWAPP AP MFP Cap. Sub-type",	/* name */

		CISCO_LWELEM_AP_MFP_CAP_SUBTYPE,				/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		
		5,5,					/* min/max length */
		CW_TYPE_DWORD,				/* type */
		"cisco/ap-mfp-cap-subtype",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco AP MFP Config Subt.",			/* name */
		CISCO_LWELEM_AP_MFP_CONFIG_SUBTYPE,				/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/lwelem14",			/* Key */
		cw_in_generic,			/* get */
		cw_out_generic			/* put */
	},



	{ 
		"Cisco LWAPP PHY HT Cap.",			/* name */

		CISCO_LWELEM_PHY_HT_CAP,				/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		
		1,1024,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/phy-ht-cap",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,		/* put */
		NULL,
		NULL,
		cisco_phy_ht_cap_stru,
	},


	{ 
		"Cisco LWAPP Station Cfg 28 ???",			/* name */

		CISCO_LWELEM_28,				/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		
		1,1024,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/station-cfg",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			 /* put */
		NULL,
		NULL,
		cisco_station_cfg_stru,
	},

	{ 
		"Cisco LWAPP PHY HT Control",		/* name */

		CISCO_LWELEM_PHY_HT_CONTROL,				/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		
		1,1024,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/phy-ht-control",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_phy_ht_control_stru
	},



	{ 
		"Cisco LWAP Elem 33",			/* name */

		CISCO_LWELEM_33,				/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/lwelem33",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco LWAP Elem 48",			/* name */

		CISCO_LWELEM_48,				/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/lwelem48",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco LWAP Elem 55",			/* name */

		CISCO_LWELEM_55,				/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/lwelem55",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco LWAPP Radio Module Info",	/* name */
		CISCO_LWELEM_RADIO_MODULE_INFO,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		183,183,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/radio-module-info",		/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_lw_radio_module_info_stru	/* param */
	},




	{ 
		"Cisco LWAPP Elem 105",			/* name */

		CISCO_LWELEM_105,				/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/lwelem105",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},






	{ 
		"Cisco Elem 24",			/* name */
		CISCO_ELEM_24,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem24",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},




	{ 
		"Cisco Elem 19",			/* name */
		CISCO_ELEM_19,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem19",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},



	{ 
		"Cisco Elem 22",			/* name */
		CISCO_ELEM_22,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem22",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},

	{ 
		"Cisco - Performance Profile",		/* name */
		CISCO_ELEM_PERFORMANCE_PROFILE,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		18,18,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/proformance-profile",		/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_performance_profile_stru
	},




	{ 
		"Cisco Elem 81",			/* name */
		CISCO_ELEM_81,				/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1024,					/* min/max length */
		CW_TYPE_BSTR16,				/* type */
		"cisco/elem81",				/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic			/* put */
	},







	{ 
		"AP Quality of Service",		/* name */
		CISCO_ELEM_AP_QOS,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		52,52,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-qos",				/* Key */
		cw_in_radio_generic,		/* get */
		cw_out_radio_generic, /* put */
		NULL,
		NULL,
		cisco_ap_qos,
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
		"capwap/location-data",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic				/* put */
	},

	{ 
		"AP Core Dump",			/* name */
		CISCO_ELEM_AP_CORE_DUMP,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		0,1024,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-core-dump",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_core_dump			/* param */

	},
	
	{ 
		"Statitsics Timer",			/* name */
		CISCO_ELEM_STATISTICS_TIMER,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		2,2,					/* min/max length */
		CW_TYPE_WORD,				/* type */
		"capwap/statistics-timer",		/* Key */
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
		"capwap/ac-name-with-priority",		/* Key */
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
		CW_TYPE_STRUCT,				/* type */
		"cisco/vlan",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_vlan
	}
	,


	{ 
		"TCP Adjust MSS",			/* name */
		CISCO_LWELEM_TCP_ADJUST_MSS,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		3,3,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/rouge-and-mss",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_rouge_and_mss,
	}
	,


	{ 
		"Rouge Detection 7.0",			/* name */
		CISCO_LWELEM_ROUGE_DETECTION,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		5,5,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/rouge-detection",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_rouge_detection70,
	}
	,

	{ 
		"Add Cisco WLAN",			/* name */
		CISCO_ELEM_ADD_WLAN,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		7,1117,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"radio/wlan/add-wlan",			/* Key */
		cw_in_generic,				/* get */
		cw_out_traverse,			/* put */
		cisco_add_wlan_mkkey70,
		NULL, //		cisco_patch_add_wlan70
		cisco_add_wlan70
	}
	,
	
	{ 
		"Add Cisco WLAN (LWAPP)",		/* name */
		CISCO_LWELEM_ADD_WLAN,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		7,1117,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"radio/wlan/add-lw-wlan",		/* Key */
		cw_in_generic,				/* get */
		cw_out_traverse,			/* put */
		cisoc_add_lwwlan_mkkey,
		cisco_patch_add_lwwlan,
		cisco_add_lwwlan,
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
		"SSC Hash",				/* name */
		CISCO_LWELEM_SSC_HASH,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		1,331,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ssc-hash",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ssc_hash,
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
		CW_TYPE_STRUCT,				/* type */
		"capwap/operational-state",		/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,					/* mkkey */
		NULL,
		cisco_radio_oper_state			/* paam */

	}
	,

	{ 
		"Hardware Info",			/* name */
		CISCO_LWELEM_HARDWARE_INFO,		/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		17,417,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/hardware-info",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_hardware_info,

	},

	{ 
		"Cisco Discovery Protocol",		/* name */
		CISCO_LWELEM_DISCOVERY_PROTOCOL,	/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		3,3,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/cisco-discovery-protocol",	/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_discovery_protocol,


	},

	{ 
		"RAD Extended Config",			/* name */
		CISCO_LWELEM_RAD_EXTENDED_CONFIG,	/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		13,13,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/rad-extended-config",		/* Key */
		cw_in_radio_generic,		/* get */
		cw_out_radio_generic,		/* put */
		NULL,
		NULL,
		cisco_rad_extended_config,
	},

	{
		"CAPWAP Timers (Cisco)",		/* name */
		CISCO_ELEM_CAPWAP_TIMERS,		/* Element ID */
		CW_VENDOR_ID_CISCO, 0,			/* Vendor / Proto */
		2, 2,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"capwap/timers",			/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_capwap_timers,			/* param */
	}
	,

	{
		"8021.11 Assoc Limit (Cisco)",			/* name */
		CISCO_ELEM_80211_ASSOC_LIMIT,			/* Element ID */
		CW_VENDOR_ID_CISCO, 0,						/* Vendor / Proto */
		4, 4,						/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/8011-assoc-limit",			/* Key */
		cw_in_generic,					/* get */
		cw_out_generic,					/* put */
		NULL,
		NULL,
		cisco_80211_assoc_limit
	}
	,

	{
		"CISCO Sig Toggle",				/* name */
		CISCO_ELEM_SIG_TOGGLE,				/* Element ID */
		CW_VENDOR_ID_CISCO, 0,				/* Vendor / Proto */
		1, 1,						/* min/max length */
		CW_TYPE_BOOL,					/* type */
		"cisco/sig-toggle",				/* Key */
		cw_in_generic,					/* get */
		cw_out_generic					/* put */
	}
	,

	{
		"Dot11r WLC Mac And IP (Cisco)",		/* name */
		CISCO_LWELEM_DOT11R_WLC_MAC_AND_IP,		/* Element ID */
		0, 0,						/* Vendor / Proto */
		4, 4,						/* min/max length */
		CW_TYPE_STRUCT,					/* type */
		"cisco/dot11r-wlc-mac-and-ip",			/* Key */
		cw_in_generic,					/* get */
		cw_out_generic,					/* put */
		NULL,
		NULL,
		cisco_dot11r_wlc_mac_and_ip
	}
	,
	
	{
		"AP DTLS Data CFG",				/* name */
		CISCO_LWELEM_AP_DTLS_DATA_CFG,			/* Element ID */
		CW_VENDOR_ID_CISCO, CW_PROTO_LWAPP,		/* Vendor / Proto */
		2, 2,						/* min/max length */
		CW_TYPE_STRUCT,					/* type */
		"cisco/ap-dtls-data-cfg",			/* Key */
		cw_in_generic,					/* get */
		cw_out_generic,					/* put */
		NULL,
		NULL,
		dtls_data_cfg
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
	
	{
		"MCAST MGID Info",				/* name */
		CISCO_LWELEM_MCAST_MGID_INFO,			/* Element ID */
		CW_VENDOR_ID_CISCO, CW_PROTO_LWAPP,		/* Vendor / Proto */
		4, 334,						/* min/max length */
		CW_TYPE_BSTR16,					/* type */
		"cisco/mcast-mgid-info",			/* Key */
		cw_in_generic,					/* get */
		cw_out_generic					/* put */
	}
	,
	{0,0,0,0,0,0,0,0}

};


static struct cw_ElemDef discovery_request_elements[] ={
/*	{0,0,			CAPWAP_ELEM_WTP_DESCRIPTOR,	1, 0},*/
	{0,0,			CAPWAP_ELEM_WTP_BOARD_DATA,	0, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_RAD_NAME,		1, 0},
	{0,CW_VENDOR_ID_CISCO,	CW_CISCO_BOARD_DATA_OPTIONS,	0, 0},
	{0,0,			CAPWAP80211_ELEM_WTP_RADIO_INFORMATION,	1, 0},
	{0,0,0,00}
	
};

/*static uint16_t discovery_response_states[] = {CAPWAP_STATE_DISCOVERY,0};*/
static struct cw_ElemDef discovery_response_elements[] ={
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_TIMESYNC,			1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_MWAR_TYPE,			0, 0},
	{0,0,			CAPWAP80211_ELEM_WTP_RADIO_INFORMATION,	1, 0},
	{0,0,0,00}
	
};

/*static uint16_t join_request_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef join_request_elements[] ={
	{0,0,			CAPWAP_ELEM_WTP_IPV4_IP_ADDRESS,1,0},
	{0,0,			CAPWAP_ELEM_WTP_IPV6_IP_ADDRESS,1,0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_GROUP_NAME,	1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_MWAR_ADDR,		1, 0},
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},

	
	{0,CW_VENDOR_ID_CISCO,	CW_CISCO_BOARD_DATA_OPTIONS,	1, 0},
	{0,0,CAPWAP_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS,		0, CW_DELETE},
	{0,0,CAPWAP_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS,		0, CW_DELETE},
	{0,0,			CAPWAP_ELEM_ECN_SUPPORT,	0, CW_DELETE},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PATH_MTU,	0, 0},

	{0,0,0,00}
	
};

/*static uint16_t join_response_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef join_response_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},

	{0,0,			CAPWAP_ELEM_ECN_SUPPORT,	0, CW_DELETE},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PATH_MTU,	1, 0},
	{0,0,0,00}
	
};


/*static uint16_t configuration_status_request_states[] = {CAPWAP_STATE_JOIN,CAPWAP_STATE_RUN,0};*/

static cw_State_t configuration_status_request_states[] = {
	{CAPWAP_STATE_JOIN, CAPWAP_STATE_CONFIGURE},
	{CAPWAP_STATE_RUN, CAPWAP_STATE_RUN},
	{0,0}
};

static struct cw_ElemDef configuration_status_request_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_15,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SUPPORTED_RATES,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_19,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_22,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_24,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_RRM_LOAD,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_33,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_39,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_PERFORMANCE_PROFILE,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_CFP_STATUS,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_81,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_132,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_145,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_146,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_153,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_156,				0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_9,		0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_MFP_CAP_SUBTYPE,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_MFP_CONFIG_SUBTYPE,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PHY_HT_CAP,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_28,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PHY_HT_CONTROL,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_33,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_48,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_55,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_105,	0, 0},	

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_CAPWAP_TIMERS,		0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_DIRECT_SEQUENCE_CONTROL,	0, 0},


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
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_DOMAIN,			0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_DNS,			0, 0},

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_MIN_IOS_VERSION,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_BACKUP_SOFTWARE_VERSION,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_REGULATORY_DOMAIN,	1, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_MODEL,			1, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_RESET_BUTTON_STATE,		1, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_WTP_RADIO_CONFIGURATION,	1, 0},

	{0, CW_VENDOR_ID_CISCO,	CW_CISCO_ANTENNA_PAYLOAD,		1, 0},

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AC_NAME_WITH_INDEX,		0, CW_IGNORE},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_CORE_DUMP,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_VENUE_SETTINGS,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_MAC_OPERATION,		0, 0},

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_TX_POWER,			1, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_TX_POWER_LEVELS,		1, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_CHANNEL_POWER,		1, 0},	

	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_USERNAME_PASSWORD,	1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_LOGHOST_CONFIG,		1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_TELNET_SSH,		1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_SUBMODE,		1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_ETHERNET_PORT_SUBTYPE,	1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AC_IP_ADDR_WITH_INDEX,	0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_VLAN,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_TCP_ADJUST_MSS,		0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_HARDWARE_INFO,		0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_ROUGE_DETECTION,		0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_DTLS_DATA_CFG,		0, 0},
//	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PATH_MTU,			0, 0},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_DISCOVERY_PROTOCOL,	1, 0},
	{0,0,0,00}
	
};

/*static uint16_t configuration_status_response_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef configuration_status_response_elements[] ={


	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_15,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SUPPORTED_RATES,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_19,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_22,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_24,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_RRM_LOAD,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_33,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_39,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_PERFORMANCE_PROFILE,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_CFP_STATUS,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_81,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_132,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_145,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_146,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_153,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_156,				0, 0},	

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_9,		0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_MFP_CAP_SUBTYPE,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_MFP_CONFIG_SUBTYPE,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PHY_HT_CAP,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_28,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PHY_HT_CONTROL,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_33,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_48,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_55,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_105,	0, 0},	
	

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_CAPWAP_TIMERS,		0, 0},	
	

	{0,0,			CAPWAP_ELEM_RADIO_ADMINISTRATIVE_STATE,	1,0},
	{0,0,			CAPWAP_ELEM_RADIO_OPERATIONAL_STATE,	1,0},
	
	{0,CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},
	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_MULTI_DOMAIN_CAPABILITY,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_WTP_RADIO_CONFIGURATION,	1, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_DIRECT_SEQUENCE_CONTROL,	0, 0},
	{0, CW_VENDOR_ID_CISCO,	CW_CISCO_ANTENNA_PAYLOAD,		1, 0},
	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_LED_STATE_CONFIG,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_LOG_FACILITY,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_MAC_OPERATION,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_TX_POWER,			0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_QOS,			0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AIRSPACE_CAPABILITY,		0, 0},


	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_DOMAIN_SECRET,		0, 0},	

	

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_MWAR_HASH_VALUE,	1, 0},

	{0,0,0,0}
};


/*static uint16_t configuration_update_request_states[] = {CAPWAP_STATE_RUN,0};*/
static struct cw_ElemDef configuration_update_request_elements[] ={

	{0, 0,			CAPWAP_ELEM_RADIO_ADMINISTRATIVE_STATE,	0, 0},
	{0, 0,			CAPWAP_ELEM_RADIO_OPERATIONAL_STATE,	0, 0},

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_15,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SUPPORTED_RATES,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_19,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_22,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_24,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_RRM_LOAD,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_33,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_39,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_PERFORMANCE_PROFILE,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_CFP_STATUS,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_81,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_132,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_145,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_146,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_153,				0, 0},	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_156,				0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_9,		0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_MFP_CAP_SUBTYPE,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_AP_MFP_CONFIG_SUBTYPE,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PHY_HT_CAP,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_28,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PHY_HT_CONTROL,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_33,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_48,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_55,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_105,	0, 0},	
	

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_CAPWAP_TIMERS,		0, 0},	
	


	{0,0,			CAPWAP_ELEM_RADIO_OPERATIONAL_STATE,	0,0},
	{0, CW_VENDOR_ID_CISCO,	CW_CISCO_ANTENNA_PAYLOAD,		0,0},
	
	
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
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_DOMAIN,			0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_DNS,			0, 0},

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_DIRECT_SEQUENCE_CONTROL,	0, 0},
	
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
//	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_AP_VENUE_SETTINGS,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_80211_ASSOC_LIMIT,		0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SIG_TOGGLE,			0, 0},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_MAC_OPERATION,		0, 0},

	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_TX_POWER,			0, 0},	
/*	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_TX_POWER_LEVELS,		1, 0},	*/
/*	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_CHANNEL_POWER,		1, 0},	*/



	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_ADD_WLAN,			0, 0},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_MCAST_MGID_INFO,		0, 0},
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
	
	{0,0,0,0,0}
	
};

/*static uint16_t wtp_event_request_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef wtp_event_request_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,0, CW_IGNORE},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_ADD_WLAN,			0, CW_IGNORE},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_RRM_LOAD,				0, 0},	

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_ADD_WLAN,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_HARDWARE_INFO,			0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_RADIO_MODULE_INFO,	0, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_PHY_HT_CONTROL,	0, 0},	
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_55,	0, 0},	

	{0,0,0,0,0}
};

/*static uint16_t wtp_event_response_states[] = {CAPWAP_STATE_JOIN,0};*/
static struct cw_ElemDef wtp_event_response_elements[] ={
	/* Cisco APs don't like a result code in event responses */ 
	{0,0,CAPWAP_ELEM_RESULT_CODE,				1, CW_DELETE},

	{0,0,0,0,0}
};


static struct cw_ElemDef change_state_event_request_elements[] ={
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_SPAM_VENDOR_SPECIFIC,	1, CW_IGNORE},
	
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_ADD_WLAN,			1, CW_IGNORE},
	{0, CW_VENDOR_ID_CISCO,	CISCO_ELEM_OPER_STATE_DETAIL_CAUSE,	1, CW_IGNORE},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_105,	0, 0},	

	{0, 0,			CAPWAP_ELEM_RADIO_ADMINISTRATIVE_STATE,	1, 0},

	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_HARDWARE_INFO,	1, 0},
	{CW_PROTO_LWAPP, CW_VENDOR_ID_CISCO,	CISCO_LWELEM_RADIO_MODULE_INFO,	1, 0},


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


static struct cw_MsgDef messages70[] = {
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
		CAPWAP_MSG_PRIMARY_DISCOVERY_REQUEST,	/* type */
		CW_ROLE_AC,			/* role */
		NULL,				/* states */
		discovery_request_elements,	/* elements */
		NULL,				/* preprocess fun */
		postprocess_discovery		/* postprocess fun */
	},
	{
		NULL,				/* name */
		CAPWAP_MSG_PRIMARY_DISCOVERY_RESPONSE,	/* type */
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
		configuration_status_request_states,		/* states */
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
		CW_ROLE_AC,				/* role */
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

static struct cw_ElemHandler handlers73[] = {
	{ 
		"Rouge Detection (>=7.3)",				/* name */
		CISCO_LWELEM_ROUGE_DETECTION,			/* Element ID */
		CW_VENDOR_ID_CISCO,CW_PROTO_LWAPP,	/* Vendor / Proto */
		7,7,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/rouge-detection",				/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_rouge_detections,
	}

	,

	{ 
		"AP LED State Config (>= v7.3)",	/* name */
		CISCO_ELEM_AP_LED_STATE_CONFIG,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		1,1,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-led-state-config",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_led_state_config73,
	},

	{ /* WTP Radio Configuration for AC/WPT with version 7.3 */
	
		"WTP Radio Configuration (v7.3)",		/* name */
		CISCO_ELEM_WTP_RADIO_CONFIGURATION,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		27,27,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"capwap80211/wtp-radio-config",		/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_wtp_radio_config73,
	}
	,

	{ 
		"AP Venue Settings",				/* name */
		CISCO_ELEM_AP_VENUE_SETTINGS,			/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		5,1024,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/ap-venue-settings",		/* Key */
		cw_in_generic,				/* get */
		cw_out_generic,				/* put */
		NULL,
		NULL,
		cisco_ap_venue_settings,
	}
	,
	
	{0,0,0,0,0,0,0,0}
};



static struct cw_ElemHandler handlers75[] = {
	{ 
		"WTP Radio Configuration  (Version >= 7.5)",/* name */
		CISCO_ELEM_WTP_RADIO_CONFIGURATION,	/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		28,28,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"capwap80211/wtp-radio-config",		/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_wtp_radio_config75,
	}
	,
	{ 
		"Mac Operation (Version >= 7.5)",	/* name */
		CISCO_ELEM_MAC_OPERATION,		/* Element ID */
		CW_VENDOR_ID_CISCO,0,			/* Vendor / Proto */
		17,17,					/* min/max length */
		CW_TYPE_STRUCT,				/* type */
		"cisco/mac-operation",			/* Key */
		cw_in_radio_generic,			/* get */
		cw_out_radio_generic,			/* put */
		NULL,
		NULL,
		cisco_mac_operation75,
	},

	{0,0,0,0,0,0,0,0}
};




static struct cw_MsgDef messages73[] = {
	{0,0,0,0}
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


//static int (*postprocess_join_request_parent)(struct cw_Conn * conn);
static int (*postprocess_join_request_parent)(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len);

struct cw_MsgSet * cisco_register_msg_set(struct cw_MsgSet * set, int mode){

	struct cw_MsgData * md;
        
	if (mode != CW_MOD_MODE_CAPWAP)
                return NULL;
		
	md = cw_msgset_get_msgdata(set,CAPWAP_MSG_JOIN_REQUEST);
	if (md != NULL){
		postprocess_join_request_parent = md->postprocess;
	}
	
        cw_msgset_add(set,messages70, handlers70);
	cw_msgset_add_states(set,statemachine_states);
        return set;
}

static void update_msgset(struct cw_MsgSet *msgset, bstr_t version)
{
	if(bstr16_len(version)==4){
		uint32_t rv;
		rv = cw_get_dword(bstr16_data(version));
		
		if (rv >= 0x07030000){
			cw_dbg(DBG_MOD, "CISCO - Loading messages for 0x%08X >= 0x07030000", rv);
			cw_msgset_add(msgset,messages73, handlers73);
		}
		if (rv >= 0x07056600){
			cw_dbg(DBG_MOD, "CISCO - Loading messages for 0x.x%08X >= 0x07056600", rv);
			cw_msgset_add(msgset,messages75, handlers75);
		}
	}


}


static void set_ac_version(struct cw_ElemHandlerParams * params)
{
	bstr16_t wtpver;
	char verstr[512];
	wtpver = cw_cfg_get_bstr16(params->cfg,"capwap/wtp-descriptor/software/version",NULL);
	if (wtpver==NULL)
		return;

	cw_format_version(verstr,bstr16_data(wtpver),bstr16_len(wtpver));
	cw_dbg(DBG_INFO, "Cisco - Setting AC software version to: %s", verstr);
	
	cw_cfg_set_bstr16(params->conn->local_cfg,"capwap/ac-descriptor/software/version",wtpver);
	cw_cfg_set_int(params->conn->local_cfg,"capwap/ac-descriptor/software/vendor",CW_VENDOR_ID_CISCO);

	update_msgset(params->msgset,wtpver);
	free(wtpver);
}

static int postprocess_discovery(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len)
{
	if (params->conn->role == CW_ROLE_AC ){
		set_ac_version(params);
		cw_detect_nat(params);
	}
	return 1;
}

static int postprocess_join_request(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len)
{
	if (postprocess_join_request_parent!=NULL){
		postprocess_join_request_parent(params,elems_ptr,elems_len);
	}
	postprocess_discovery(params,elems_ptr,elems_len);
	return 1;
}

static int preprocess_join_request(struct cw_Conn *conn)
{
	bstr16_t ver;
	int use_ac_version;
	char verstr[512];

	if (conn->role != CW_ROLE_WTP)
		return 0;

	use_ac_version = cw_cfg_get_bool(conn->global_cfg,"mod/cisco/wtp-use-ac-version",0);
	if (use_ac_version){
		ver = cw_cfg_get_bstr16(conn->remote_cfg,"capwap/ac-descriptor/software/version",NULL );
		if (ver != NULL){
			cw_cfg_set_bstr16(conn->local_cfg,"capwap/wtp-descriptor/software/version",ver);
			cw_format_version(verstr,bstr16_data(ver),bstr16_len(ver));
			cw_dbg(DBG_MOD, "CISCO WTP - Using AC's software version: %s", verstr);
			update_msgset(conn->msgset,ver);
			free(ver);
		}
		else{
			cw_dbg(DBG_X,"No version defined");
		}

	}
	return 1;
}
