
#include "capwap.h"
#include "cipwap.h"
#include "capwap_cisco.h"


struct cw_strlist_elem cipwap_strings_elem[] = {

	{CW_ELEM_WTP_IPV4_IP_ADDRESS, "WTP IPv4 IP Address"},
	{CW_ELEM_WTP_IPV6_IP_ADDRESS, "WTP IPv6 IP Address"},
	{CW_ELEM_AC_NAME_WITH_INDEX, "AC Name with Index"},

	{CW_CISCO_AP_GROUP_NAME, "WTP Group Name" },

	{CW_STR_STOP,"Unknown Element"}
	
};
