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

#ifndef __LWAPP_CISCO_H
#define __LWAPP_CISCO_H

#include <stdint.h>

#include "lwapp.h"

//#define LW_VENDOR_CISCO				4232704


/* Vendor specific message elements LWAPP Cisco */


enum lwapp_cisco_elems {

LW_CISCO_AP_USERNAME_PASSWORD=18,

LW_CISCO_RADIO_MODULE_INFO=21,
LW_CISCO_AC_IP_ADDR_WITH_INDEX=32,
LW_CISCO_AP_ETHERNET_PORT_SUBTYPE=34,
LW_CISCO_AP_LOGHOST_CONFIG=36,
LW_CISCO_MCAST_MGID_INFO=39,

LW_CISCO_TELNET_SSH=44,
LW_CISCO_AP_SUBMODE=67,
LW_CISCO_AP_HEARTBEAT_TIMEOUT=68,

LW_CISCO_PRIMED_DISCOVERY_TIMEOUT=50,
LW_CISCO_PATH_MTU=73,
LW_CISCO_PRIMED_JOIN_TIMEOUT=85,
LW_CISCO_AP_DTLS_DATA_CFG=74,

LW_CISCO_ADD_WLAN=128,

LW_CISCO_SSC_HASH_VALIDATION=133,
LW_CISCO_MWAR_HASH_VALUE=134,
LW_CISCO_DOT11R_WLC_MAC_AND_IP=135,

LW_CISCO_HARDWARE_INFO=139,

LW_CISCO_AP_JOIN_IP_PREF_MODE=166

};
	

/* function proto types */
extern int lw_put_cisco_path_mtu(uint8_t *dst, uint16_t max, uint16_t padding);

extern const char * lw_cisco_id_to_str(int elem_id);


/*
#define lw_addelem_cisco_path_mtu(dst,max,padding) \
	lw_put_elem_hdr(dst,LW_ELEM_VENDOR_SPECIFIC,lw_put_cisco_path_mtu(dst,max,padding))
*/

#endif
