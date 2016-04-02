#ifndef __DOT11X_H
#define __DOT11X_H


#include <stdint.h>


struct dot11_mgmt_head {
	uint16_t frame_control;
	uint16_t duration;
	uint8_t da[6];
	uint8_t sa[6];
	uint8_t bssid[6];
	uint16_t seq_ctrl;
};


struct dot11_mgmt_beacon{
	uint8_t timestamp[8];
	uint16_t beacon_int;
	uint16_t capab_info;
};

#define ETH_ALEN 6
/* SA Query Action frame (IEEE 802.11w/D8.0, 7.4.9) */
#define WLAN_SA_QUERY_REQUEST 0
#define WLAN_SA_QUERY_RESPONSE 1
#define WLAN_SA_QUERY_TR_ID_LEN 2

struct dot11_mgmt {
	uint16_t frame_control;
	uint16_t duration;
	uint8_t da[6];
	uint8_t sa[6];
	uint8_t bssid[6];
	uint16_t seq_ctrl;


	union {
		struct {
			uint8_t timestamp[8];
			uint16_t beacon_int;
			uint16_t capab_info;
			/* followed by some of SSID, Supported rates,
			 * FH Params, DS Params, CF Params, IBSS Params, TIM */
			uint8_t variable[0];
		}  beacon;
		struct {
			uint16_t auth_alg;
			uint16_t auth_transaction;
			uint16_t status_code;
			/* possibly followed by Challenge text */
			uint8_t variable[0];
		}  auth;
		struct {
			uint16_t reason_code;
			uint8_t variable[0];
		}  deauth;
		struct {
			uint16_t capab_info;
			uint16_t listen_interval;
			/* followed by SSID and Supported rates */
			uint8_t variable[0];
		}  assoc_req;
		struct {
			uint16_t capab_info;
			uint16_t status_code;
			uint16_t aid;
			/* followed by Supported rates */
			uint8_t variable[0];
		}  assoc_resp, reassoc_resp;
		struct {
			uint16_t capab_info;
			uint16_t listen_interval;
			uint8_t current_ap[6];
			/* followed by SSID and Supported rates */
			uint8_t variable[0];
		}  reassoc_req;
		struct {
			uint16_t reason_code;
			uint8_t variable[0];
		}  disassoc;
		struct {
			/* only variable items: SSID, Supported rates */
			uint8_t variable[0];
		}  probe_req;
		struct {
			uint8_t timestamp[8];
			uint16_t beacon_int;
			uint16_t capab_info;
			/* followed by some of SSID, Supported rates,
			 * FH Params, DS Params, CF Params, IBSS Params */
			uint8_t variable[0];
		}  probe_resp;
		struct {
			uint8_t category;
			union {
				struct {
					uint8_t action_code;
					uint8_t dialog_token;
					uint8_t status_code;
					uint8_t variable[0];
				}  wmm_action;
				struct{
					uint8_t action_code;
					uint8_t element_id;
					uint8_t length;
					uint8_t switch_mode;
					uint8_t new_chan;
					uint8_t switch_count;
				}  chan_switch;
				struct {
					uint8_t action;
					uint8_t sta_addr[ETH_ALEN];
					uint8_t target_ap_addr[ETH_ALEN];
					uint8_t variable[0]; /* FT Request */
				}  ft_action_req;
				struct {
					uint8_t action;
					uint8_t sta_addr[ETH_ALEN];
					uint8_t target_ap_addr[ETH_ALEN];
					uint16_t status_code;
					uint8_t variable[0]; /* FT Request */
				}  ft_action_resp;
				struct {
					uint8_t action;
					uint8_t trans_id[WLAN_SA_QUERY_TR_ID_LEN];
				}  sa_query_req;
				struct {
					uint8_t action; /* */
					uint8_t trans_id[WLAN_SA_QUERY_TR_ID_LEN];
				}  sa_query_resp;
				struct {
					uint8_t action;
					uint8_t dialogtoken;
					uint8_t variable[0];
				}  wnm_sleep_req;
				struct {
					uint8_t action;
					uint8_t dialogtoken;
					uint16_t keydata_len;
					uint8_t variable[0];
				}  wnm_sleep_resp;
				struct {
					uint8_t action;
					uint8_t variable[0];
				}  public_action;
				struct {
					uint8_t action; /* 9 */
					uint8_t oui[3];
					/* Vendor-specific content */
					uint8_t variable[0];
				}  vs_public_action;
				struct {
					uint8_t action; /* 7 */
					uint8_t dialog_token;
					uint8_t req_mode;
					uint16_t disassoc_timer;
					uint8_t validity_interval;
					/* BSS Termination Duration (optional),
					 * Session Information URL (optional),
					 * BSS Transition Candidate List
					 * Entries */
					uint8_t variable[0];
				}  bss_tm_req;
				struct {
					uint8_t action; /* 8 */
					uint8_t dialog_token;
					uint8_t status_code;
					uint8_t bss_termination_delay;
					/* Target BSSID (optional),
					 * BSS Transition Candidate List
					 * Entries (optional) */
					uint8_t variable[0];
				}  bss_tm_resp;
				struct {
					uint8_t action; /* 6 */
					uint8_t dialog_token;
					uint8_t query_reason;
					/* BSS Transition Candidate List
					 * Entries (optional) */
					uint8_t variable[0];
				}  bss_tm_query;
			} u;
		}  action;
	} u;













};


/* frame types */
#define DOT11_FTYPE_MGMT		0x0000
#define DOT11_FTYPE_CTL			0x0004
#define DOT11_FTYPE_DATA		0x0008



/* management */
#define DOT11_STYPE_ASSOC_REQ		0x0000
#define DOT11_STYPE_ASSOC_RESP		0x0010
#define DOT11_STYPE_REASSOC_REQ		0x0020
#define DOT11_STYPE_REASSOC_RESP	0x0030
#define DOT11_STYPE_PROBE_REQ		0x0040
#define DOT11_STYPE_PROBE_RESP		0x0050
#define DOT11_STYPE_BEACON		0x0080
#define DOT11_STYPE_ATIM		0x0090
#define DOT11_STYPE_DISASSOC		0x00A0
#define DOT11_STYPE_AUTH		0x00B0
#define DOT11_STYPE_DEAUTH		0x00C0
#define DOT11_STYPE_ACTION		0x00D0

/* control */
#define DOT11_STYPE_BACK_REQ		0x0080
#define DOT11_STYPE_BACK		0x0090
#define DOT11_STYPE_PSPOLL		0x00A0
#define DOT11_STYPE_RTS			0x00B0
#define DOT11_STYPE_CTS			0x00C0
#define DOT11_STYPE_ACK			0x00D0
#define DOT11_STYPE_CFEND		0x00E0
#define DOT11_STYPE_CFENDACK		0x00F0

/* data */
#define DOT11_STYPE_DATA		0x0000
#define DOT11_STYPE_DATA_CFACK		0x0010
#define DOT11_STYPE_DATA_CFPOLL		0x0020
#define DOT11_STYPE_DATA_CFACKPOLL	0x0030
#define DOT11_STYPE_NULLFUNC		0x0040
#define DOT11_STYPE_CFACK		0x0050
#define DOT11_STYPE_CFPOLL		0x0060
#define DOT11_STYPE_CFACKPOLL		0x0070
#define DOT11_STYPE_QOS_DATA		0x0080
#define DOT11_STYPE_QOS_DATA_CFACK	0x0090
#define DOT11_STYPE_QOS_DATA_CFPOLL	0x00A0
#define DOT11_STYPE_QOS_DATA_CFACKPOLL	0x00B0
#define DOT11_STYPE_QOS_NULLFUNC	0x00C0
#define DOT11_STYPE_QOS_CFACK		0x00D0
#define DOT11_STYPE_QOS_CFPOLL		0x00E0
#define DOT11_STYPE_QOS_CFACKPOLL	0x00F0




/* Information Element IDs */
#define WLAN_EID_SSID 0
#define WLAN_EID_SUPP_RATES 1
#define WLAN_EID_FH_PARAMS 2
#define WLAN_EID_DS_PARAMS 3
#define WLAN_EID_CF_PARAMS 4
#define WLAN_EID_TIM 5
#define WLAN_EID_IBSS_PARAMS 6
#define WLAN_EID_COUNTRY 7
#define WLAN_EID_BSS_LOAD 11
#define WLAN_EID_CHALLENGE 16
/* EIDs defined by IEEE 802.11h - START */
#define WLAN_EID_PWR_CONSTRAINT 32
#define WLAN_EID_PWR_CAPABILITY 33
#define WLAN_EID_TPC_REQUEST 34
#define WLAN_EID_TPC_REPORT 35
#define WLAN_EID_SUPPORTED_CHANNELS 36
#define WLAN_EID_CHANNEL_SWITCH 37
#define WLAN_EID_MEASURE_REQUEST 38
#define WLAN_EID_MEASURE_REPORT 39
#define WLAN_EID_QUITE 40
#define WLAN_EID_IBSS_DFS 41
/* EIDs defined by IEEE 802.11h - END */






/* capability bits */
#define WLAN_CAPAB_ESS			0x001
#define WLAN_CAPAB_IBSS			0x002
#define WLAN_CAPAB_CF_POLLABLE 		0x004
#define WLAN_CAPAB_CF_POLL_REQUEST	0x008
#define WLAN_CAPAB_PRIVACY 		0x010
#define WLAN_CAPAB_SHORT_PREAMBLE	0x020
#define WLAN_CAPAB_PBCC 		0x040
#define WLAN_CAPAB_CHANNEL_AGILITY 	0x080
#define WLAN_CAPAB_SPECTRUM_MGMT BIT	0x100
#define WLAN_CAPAB_SHORT_SLOT_TIME BIT	0x200
#define WLAN_CAPAB_DSSS_OFDM 		0x400









struct apdata {
	int phy;
	uint32_t idx;
	uint8_t mac[6];
	const char * ssid;
};


struct beacon_data{
	uint8_t * head;
	int head_len;
	uint8_t * tail;
	int tail_len;

};


int dot11_get_beacon_data(struct apdata *ap,struct beacon_data *bd);




#endif
