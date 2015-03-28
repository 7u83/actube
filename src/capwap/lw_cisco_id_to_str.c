
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
		case LW_CISCO_AP_USERNAME_PASSWORD:
			return "AP Username and Password";
		default: 
			return "Unknown";

	}

	return "Unknown";
}

