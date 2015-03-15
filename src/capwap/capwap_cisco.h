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

#ifndef __CAPWAP_CISCO_H
#define __CAPWAP_CISCO_H

#include <string.h>

#include "lwapp.h"

#define CWVENDOR_CISCO_MWAR_ADDR			2
#define CWVENDOR_CISCO_RAD				3
#define CWVENDOR_CISCO_RAD_SLOT				4
#define CWVENDOR_CISCO_RAD_NAME				5
#define	CWVENDOR_CISCO_MWAR				6

#define CWVENDOR_CISCO_BOARD DATA			LWMSGELEM_WTP_BOARD_DATA	/* 50 */
#define CWVENDER_CISCO_AP_MODE_AND_TYPE			54

#define CWVENDOR_CISCO_SPAM_VENDOR_SPECIFIC		104

#define CWVENDOR_CISCO_AP_GROUP_NAME			123
#define CWVENDOR_CISCO_AP_LED_STATE_CONFIG		125


#define CWVENDOR_CISCO_AP_PRE_STD_SWITCH_CONFIG		137
#define CWVENDOR_CISCO_AP_POWER_INJECTOR_CONFIG		138

#define CWVENDOR_CISCO_AP_TIMESYNC			151

#define CWVENDOR_CISCO_PL207				207


extern void cwmsg_addelem_vendor_cisco_ap_timesync(struct cwmsg * cwmsg);


/* rad_name payload */
#define cwmsg_addelem_vendor_cisco_rad_name(cwmsg,str) \
	cwmsg_addelem_vendor_specific_payload(cwmsg,CW_VENDOR_ID_CISCO, \
					CWVENDOR_CISCO_RAD_NAME,str,strlen((char*)str))
/* group name payload */
#define cwmsg_addelem_vendor_cisco_group_name(cwmsg,str) \
	cwmsg_addelem_vendor_secific_payload(cwmsg,CW_VENDOR_ID_CISCO, \
					CWVENDOR_CISCO_GROUP_NAME,str,strlen((char*)str));


/* payload 207 */

#endif
