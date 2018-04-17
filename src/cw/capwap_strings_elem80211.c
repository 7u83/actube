
#include "capwap80211.h"
#include "strlist.h"

struct cw_StrListElem capwap_strings_elem80211[] = {

	{CW_ELEM80211_ADD_WLAN, "802.11 Add WLAN"},
	{CW_ELEM80211_ANTENNA, "802.11 Antenna"},
	{CAPWAP80211_ELEM_RATE_SET, "802.11 Rate Set"},
	{CW_ELEM80211_SUPPORTED_RATES,"802.11 Supported Rates"},

	{CAPWAP80211_ELEM_WTP_RADIO_INFORMATION, " 802.11 WTP Radio Information"},
	{CW_STR_STOP, " Unknown "}

};
