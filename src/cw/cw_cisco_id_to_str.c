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

		case CW_CISCO_AP_MODE_AND_TYPE:
			return "AP Mode and Type";

		case CW_CISCO_ADD_WLAN:
			return "Add WLAN";

		case CW_CISCO_MWAR_ADDR:
			return "MWAR Addr";

		case CW_CISCO_BOARD_DATA_OPTIONS:
			return "WTP Board Data Options";
		case CW_CISCO_AP_RESET_BUTTON_STATE:
			return "Reset Button State";

		case CW_CISCO_MAC_OPERATION:
			return "Mac Operation";

		case CW_CISCO_TX_POWER:
			return "TX Power";
		case CW_CISCO_TX_POWER_LEVELS:
			return "TX Power Levels";
		case CW_CISCO_DIRECT_SEQUENCE_CONTROL:
			return "Direct Sequence Control";
		case CW_CISCO_AP_MODEL:
			return "AP Model";
		case CW_CISCO_AP_MINIOS_VERSION:
			return "AP MiniOS Version";
		case CW_CISCO_LWAPP_CHANNEL_POWER:
			return "LWAPP Channel Power";
		case CW_CISCO_ANTENNA_PAYLOAD:
			return "Antenna Payload";
		case CW_CISCO_SUPPORTED_RATES:
			return "Supported Rates";
		case CW_CISCO_SPAM_DOMAIN_SECRET:
			return "SPAM Domain Secret";

		case CW_CISCO_AP_LED_STATE_CONFIG:
			return "SPAM AP LED State";
		case CW_CISCO_AP_RETRANSMIT_PARAM:
			return "AP Retransmit Param";
		case CW_CISCO_AP_VENUE_SETTINGS:
			return "AP Venue Settings";
		case CW_CISCO_80211_DELETE_WLAN:
			return "Delete WLAN";
		case CW_CISCO_SIG_PAYLOAD:
			return "Signature Payload";
		case CW_CISCO_SIG_TOGGLE:
			return "Signature Toggle";
		case CW_CISCO_80211_ASSOC_LIMIT:
			return "Assoc Limit";
		case CW_CISCO_AP_QOS:
			return "AP Quality of Services";
		case CW_CISCO_TLV_PAYLOAD:
			return "TLV Payload";
		case CW_CISCO_AP_UPTIME:
			return "AP Uptime";
		case CW_CISCO_AP_POWER_INJECTOR_CONFIG:
			return "AP Power Injector Config";
		case CW_CISCO_AP_PRE_STD_SWITCH_CONFIG:
			return "AP Pre Std Switch Config";
		case CW_CISCO_AP_BACKUP_SOFTWARE_VERSION:
			return "Backup Software Version";
		case CW_CISCO_AP_LOG_FACILITY:
			return "AP Log Facility";

		case CW_CISCO_CAPWAP_TIMERS:
			return "Capwap Timers";

		case CW_CISCO_AP_CORE_DUMP:
			return "AP Core Dump";

		case CW_CISCO_AIRSPACE_CAPABILITY:
			return "Airspace Capability";	
		case CW_CISCO_BCAST_SSID_MODE:
			return "Broadcast SSID Mode";
		case CW_CISCO_AP_LED_FLASH_CONFIG:
			return "Spam AP LED Flash Config";
		
		default:
			return "Unknown";

	}
}

