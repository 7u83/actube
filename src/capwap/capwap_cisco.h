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
 * @brief Cisco specific CAPWAP definitions 
 */

#ifndef __CAPWAP_CISCO_H
#define __CAPWAP_CISCO_H

#include <string.h>
#include <time.h>

#include "lwapp.h"
#include "capwap.h"

#define CWVENDOR_CISCO_MWAR_ADDR			2
#define CWVENDOR_CISCO_RAD				3
#define CW_CISCO_RAD_SLOT				4
#define CW_CISCO_RAD_NAME				LW_ELEM_WTP_NAME	/* 5 */
#define	CW_CISCO_MWAR					LW_ELEM_AC_DESCRIPTOR	/* 6 */

#define CWVENDOR_CISCO_BOARD DATA			LW_ELEM_WTP_BOARD_DATA	/* 50 */
#define CWVENDER_CISCO_AP_MODE_AND_TYPE			54

#define CWVENDOR_CISCO_AP_IP_ADDR			83

#define CWVENDOR_CISCO_SPAM_VENDOR_SPECIFIC		104

#define CW_CISCO_AP_GROUP_NAME				123
#define CWVENDOR_CISCO_AP_LED_STATE_CONFIG		125
#define CW_ELEM_CISCO_AP_REGULATORY_DOMAIN		126

#define CWVENDOR_CISCO_AP_PRE_STD_SWITCH_CONFIG		137
#define CWVENDOR_CISCO_AP_POWER_INJECTOR_CONFIG		138

#define CW_CISCO_AP_TIMESYNC				151

#define CW_CISCO_BOARD_DATA_OPTIONS			207
#define CW_CISCO_MWAR_TYPE				208



/**
 * Add a Cisco AP Timesync message element to a buffer
 * @param dst destination buffer
 * @param time a unix timestamp
 * @param type of time
 * @return number of bytes put (15)
 */
static inline int cw_addelem_cisco_ap_timesync(uint8_t * dst, time_t time, uint8_t type)
{
	cw_put_dword(dst + 10, time);
	cw_put_byte(dst + 14, type);
	return 5 + cw_put_elem_vendor_hdr(dst, CW_VENDOR_ID_CISCO, CW_CISCO_AP_TIMESYNC, 5);
}

/**
 * Add a Cisco RAD Name message element to buffer
 * @param dst destination buffer
 * @param name RAD name, zero terminated
 * @return number of bytes put
 */
static inline int cw_addelem_cisco_rad_name(uint8_t * dst, uint8_t * name)
{
	return cw_addelem_vendor_specific_payload(dst, CW_VENDOR_ID_CISCO, CW_CISCO_RAD_NAME, name,
						  strlen((char *) name));
}

/**
 * Add a Cisco Group Name message element to buffer
 * @param dst destination buffer
 * @param name Group name, zero terminated
 * @return number of bytes put
 */
static inline int cw_addelem_cisco_ap_group_name(uint8_t * dst, uint8_t * name)
{
	return cw_addelem_vendor_specific_payload(dst, CW_VENDOR_ID_CISCO, CW_CISCO_AP_GROUP_NAME, name,
						  strlen((char *) name));
}


/**
 * Add a Cisco MWAR message element.
 * @param dst destinnation buffer
 * @param acinfo AC data
 * @return number of bytes put
 * 
 * This message elemet is basically an LWAPP AC Descriptor 
 * encapsulated in a CAPWAP vendor specific payload message
 */
static inline int cw_addelem_cisco_mwar(uint8_t *dst, struct ac_info *acinfo){
	int l = lw_put_ac_descriptor(dst+10,acinfo);
	return l+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_MWAR,l);
}



/*
#define cw_addelem_cisco_rad(dst,acinfo)\
	lw_put_ac_descriptor(dst+10,acinfo)
	 cw_put_elem_vedor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_RAD,
*/



/* cwmsg methods */
#define cwmsg_addelem_cisco_ap_timesync(cwmsg,time,type)\
	(cwmsg)->pos+=cw_addelem_cisco_ap_timesync(((cwmsg)->msgelems+(cwmsg)->pos),time,type)

#define cwmsg_addelem_cisco_rad_name(cwmsg,name)\
	(cwmsg)->pos+=cw_addelem_cisco_rad_name(((cwmsg)->msgelems+(cwmsg)->pos),(name))

#define cwmsg_addelem_cisco_ap_grpoup_name(cwmsg,name)\
	(cwmsg)->pos+=cw_addelem_ap_group_name(((cwmsg)->msgelems+(cwmsg)->pos),(name))

#define cwmsg_addelem_cisco_mwar(cwmsg,acinfo)\
	(cwmsg)->pos+=cw_addelem_cisco_mwar(((cwmsg)->msgelems+(cwmsg)->pos),(acinfo))




#endif
