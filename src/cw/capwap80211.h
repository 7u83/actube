#ifndef __CAPWAP80211_H
#define __CAPWAP80211_H

//#include "capwap.h"
//#include <stdint.h>

//#include "bstr.h"
//#include "action.h"

/**
 *@file 
 *@brief Definition for CAPWAP IEEE 802.11 bindings as 
 *specified in RFC 5416
 */

/** 
 *@defgroup CAPWAP80211 802.11 Bindings
 *@{
 */

#define CW80211IANA_ENTERPRISE_NUMBER		 13277

#define CWMSG_80211_WLAN_CONFIGURATION_REQUEST	3398913
#define CWMSG_80211_WLAN_CONFIGURATION_REPONSE	3398914

/**
 *@defgroup Msg Elements
 *@{
 */

/** CAPWAP 802.11 Message Elements */
enum radioelems {
	/**IEER 802.11 Add WLAN Message element */
	CW_ELEM80211_ADD_WLAN = 1024,
	/** Antenna Message element */
	CW_ELEM80211_ANTENNA = 1025,
	/* IEEE 802.11 Assigned WTP BSSID */
	CW_ELEM80211_ASSIGNED_WTP_BSSID = 1026,
	/** IEEE 802.11 Delete WLAN */
	CW_ELEM80211_DELETE_WLAN = 1027,
	/** IEEE 802.11 Direct Sequence Control  */
	CW_ELEM80211_DIRECT_SEQUENCE_CONTROL = 1028,
	/** IEEE 802.11 Information Element **/
	CW_ELEM80211_INFORMATION_ELEMENT = 1029,
	/** IEEE 802.11 MAC Operation */
	CW_ELEM80211_MAC_OPERATION = 1030,
	/** IEEE 802.11 MIC Countermeasures */
	CW_ELEM80211_MIC_CPUNTERMEASURES = 1031,
	/** IEEE 802.11 Multi-Domain Capability */
	CW_ELEM80211_MULTI_DOMAIN_CAPABILITY = 1032,
	/** IEEE 802.11 OFDM Control */
	CW_ELEM80211_ODFM_CONTROL = 1033,
	/** IEEE 802.11 Supported WLAN Rates */
	CW_ELEM80211_RATE_SET = 1034,

/*   IEEE 802.11 RSNA Error Report From Station         1035
   IEEE 802.11 Station                                1036
   IEEE 802.11 Station QoS Profile                    1037
   IEEE 802.11 Station Session Key                    1038
   IEEE 802.11 Statistics                             1039
   IEEE 802.11 Supported Rates                        1040
   IEEE 802.11 Tx Power                               1041
   IEEE 802.11 Tx Power Level                         1042
   IEEE 802.11 Update Station QoS                     1043
   IEEE 802.11 Update WLAN                            1044
   IEEE 802.11 WTP Quality of Service                 1045
   IEEE 802.11 WTP Radio Configuration                1046
   IEEE 802.11 WTP Radio Fail Alarm Indication        1047
*/

	/** IEEE 802.11 Radio Information Message Element */
	CW_ELEM80211_WTP_RADIO_INFORMATION = 1048
};

/**
 *@}
 */



#define CW_80211_RADIO_TYPE_B		(1)
#define CW_80211_RADIO_TYPE_A		(2)
#define CW_80211_RADIO_TYPE_G		(4)
#define CW_80211_RADIO_TYPE_N		(8)




/*
struct cw_wlan {
	uint8_t rid; // Radio ID 
	uint8_t wid; // WLAN ID 
	uint16_t capab; 
	uint8_t key_index;
	uint8_t key_status;
	uint16_t key_length;
	uint8_t * key;
	uint8_t group_tsc[6];
	uint8_t qos;
	uint8_t auth_type;
	uint8_t mac_mode;
	uint8_t tunnel_mode;
	uint8_t suppress_ssid;
	bstr_t ssid;
};

*/

//int cw_out_radio_infos(struct conn *conn, struct cw_action_out *a, uint8_t * dst);


//extern int cw_register_actions_capwap_80211_wtp(struct cw_actiondef *def);
extern struct cw_strlist_elem capwap_strings_elem80211[];
//extern int cw_register_actions_capwap_80211_ac(struct cw_actiondef *def);

/**
 *@}
 */



#endif
