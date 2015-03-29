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


/**
 * @file
 * @brief Implements cw_cisco_id_to_str
 */ 

#include "capwap_cisco.h"

/**
 * Get a text representation of a Cisco vendor specific message element
 * @param elem_id vendor specific message element
 * @return pointer printable string
 */ 
const char * cw_cisco_id_to_str(int elem_id)
{
	switch (elem_id) {
		case CW_CISCO_AP_GROUP_NAME:
			return "AP Group Name";

		case CW_CISCO_RAD_NAME:
			return "RAD Name";

		case CW_CISCO_AP_TIMESYNC:
			return "AP Timesync";

		case CW_CISCO_MWAR_TYPE:
			return "MWAR Type";

		case CW_CISCO_SPAM_VENDOR_SPECIFIC:
			return "LWAPP Vendor Specific";
		case CW_CISCO_WTP_RADIO_CFG:
			return "WTP Radio CFG";
		case CW_CISCO_AP_REGULATORY_DOMAIN:
			return "AP Regulatory Domain";
		case CW_CISCO_WTP_BOARD_DATA:
			return "WTP Board Data";

		case CW_CISCO_MULTI_DOMAIN_CAPAB:
			return "Multi Domain Capability";

		case CW_CISCO_AP_STATIC_IP_ADDR:
			return "AP Static IP Addr";
		case CW_CISCO_AP_DOMAIN:
			return "AP Domain";
		case CW_CISCO_AP_DNS:
			return "AP DNS";
		case CW_CISCO_AC_NAME_WITH_INDEX:
			return "AC Name with Index";
		case CW_CISCO_AC_IPV4_LIST:
			return "AC IPv4 List";

		case CW_CISCO_LOCATION_DATA:
			return "Location Data";
		case CW_CISCO_STATISTICS_TIMER:
			return "Statistics Timer";

		default:
			return "Unknown";

	}
}

