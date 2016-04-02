
#include "lwapp_cisco.h"

const char * lw_cisco_id_to_str(int elem_id)
{
	switch (elem_id){
		case LW_CISCO_AP_LOGHOST_CONFIG:
			return "AP Loghost Config and Last Joined Controller";
		case LW_CISCO_PATH_MTU:
			return "Path MTU";
		case LW_CISCO_MWAR_HASH_VALUE:
			return "MWAR Hash Value";
		case LW_CISCO_MWAR_HASH_VALUE_1:
			return "MWAR Hah Value 1";			
		case LW_CISCO_AP_USERNAME_PASSWORD:
			return "AP Username and Password";
		case LW_CISCO_AC_IP_ADDR_WITH_INDEX:
			return "AC IP Addr with Index";
		case LW_CISCO_AP_SUBMODE:
			return "AP Submode";
		case LW_CISCO_SSC_HASH_VALIDATION:
			return "SSC Hash Validation";

		case LW_CISCO_DOT11R_WLC_MAC_AND_IP:
			return "802.11r WLC MAC and IP";
		case LW_CISCO_AP_JOIN_IP_PREF_MODE:
			return "AP Join IP Pref Mode";
			
		case LW_CISCO_MCAST_MGID_INFO:
			return "MCAST MGID Info";
		case LW_CISCO_RADIO_MODULE_INFO:
			return "Radio Module Info";
		case LW_CISCO_TELNET_SSH:
			return "Telnet SSH";
		case LW_CISCO_AP_DTLS_DATA_CFG:
			return "AP DTLS Data Config";

		case LW_CISCO_PRIMED_JOIN_TIMEOUT:
			return "Primed Join Timeout";			

		case LW_CISCO_PRIMED_DISCOVERY_TIMEOUT:
			return "Primed Discovery Timeout";



		default: 
			return "Unknown";

	}

	return "Unknown";
}

