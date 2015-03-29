
#include "lwapp.h"



const char * lw_elem_id_to_str(int elem_id)
{

	switch (elem_id){
		case LW_ELEM_AC_ADDRESS:
			return "AC Address";

		case LW_ELEM_WTP_DESCRIPTOR:
			return "WTP Descriptor";
		
		case LW_ELEM_WTP_NAME:
			return "WTP Name";

		case LW_ELEM_AC_DESCRIPTOR:
			return "AC Descriptor";

		case LW_ELEM_AC_NAME:
			return "AC Name";


		case LW_ELEM_SUPPORTED_RATES:
			return "Supported Rates";

		case LW_ELEM_TEST:
			return "Test";


		case LW_ELEM_CERTIFICATE:
			return "Certificate";

		case LW_ELEM_WTP_BOARD_DATA:
			return "WTP Board Data";

		case LW_ELEM_AP_IP_ADDR:
			return "AP IP Addr";

		case LW_ELEM_VENDOR_SPECIFIC:
			return "Vendor Specific";


		case LW_ELEM_80211_WTP_WLAN_RADIO_CONFIGURATION:
			return "80.211 WTP Wlan Radio Configuration";

		case LW_ELEM_80211_WTP_MODE_AND_TYPE:
			return "802.11 WTP Mode and Type";



		default:
			return "Unknown";

	}

}

