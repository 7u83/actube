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
		case CW_CISCO_STATION_CFG:
			return "Station CFG";
		case CW_CISCO_AP_REGULATORY_DOMAIN:
			return "AP Regulatory Domain";
		case CW_CISCO_WTP_BOARD_DATA:
			return "WTP Board Data";

		default:
			return "Unknown";

	}
}

