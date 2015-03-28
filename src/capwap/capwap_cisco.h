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

#define CW_CISCO_MWAR_ADDR			2
#define CW_CISCO_RAD				3
#define CW_CISCO_RAD_SLOT			4
#define CW_CISCO_RAD_NAME			LW_ELEM_WTP_NAME			/* 5 */
#define	CW_CISCO_MWAR				LW_ELEM_AC_DESCRIPTOR			/* 6 */
#define CW_CISCO_STATION_CFG			8

#define CW_CISCO_MULTI_DOMAIN_CAPAB		LW_ELEM_80211_MULTI_DOMAIN_CAPABILITY	/* 10 */

#define CW_CISCO_CERTIFICATE			LW_ELEM_CERTIFICATE			/* 44 */
#define CW_CISCO_WTP_BOARD_DATA			LW_ELEM_WTP_BOARD_DATA			/* 50 */
#define CW_CISCO_AP_MODE_AND_TYPE		LW_ELEM_80211_WTP_MODE_AND_TYPE		/* 54 */

#define CW_CISCO_AP_IP_ADDR			83

#define CW_CISCO_SPAM_VENDOR_SPECIFIC		104

#define CW_CISCO_AP_GROUP_NAME			123

#define CW_CISCO_AP_LED_STATE_CONFIG		125
#define CW_CISCO_AP_REGULATORY_DOMAIN		126

#define CW_CISCO_AP_PRE_STD_SWITCH_CONFIG	137
#define CW_CISCO_AP_POWER_INJECTOR_CONFIG	138

#define CW_CISCO_AP_TIMESYNC			151

#define CW_CISCO_BOARD_DATA_OPTIONS		207
#define CW_CISCO_MWAR_TYPE			208



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
static inline int cw_addelem_cisco_ap_group_name(uint8_t * dst, uint8_t * name){
	return cw_addelem_vendor_specific_payload(dst, CW_VENDOR_ID_CISCO, CW_CISCO_AP_GROUP_NAME, name,
						  strlen((char *) name));
}



static inline int cw_addelem_cisco_ap_regulatory_domain(uint8_t *dst, struct radioinfo * ri){
	uint8_t *d=dst+10;
	
	d+=cw_put_byte(d,ri->rid);	/* Band ID */
	d+=cw_put_byte(d,1);		/* Set True/False */
	d+=cw_put_byte(d,ri->rid);	/* Slot ID */
	d+=cw_put_word(d,ri->regDomain);
	return 5 + cw_put_elem_vendor_hdr(dst, CW_VENDOR_ID_CISCO, CW_CISCO_AP_REGULATORY_DOMAIN, 5);
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

/** 
 * Add a Cisco Certificate payload message element
 * @param dst destination buffer
 * @param src pointer to DER certificate
 * @param len length of certificate
 * @return number of bytes put
 */
static inline int cw_addelem_cisco_certificate(uint8_t*dst,uint8_t*src,int len){
	int l = lw_put_certificate(dst+10,src,len);
	return l+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_CERTIFICATE,l);
}



static inline int cw_addelem_cisco_station_cfg(uint8_t * dst,struct radioinfo *ri){
	int l = lw_put_80211_wtp_wlan_radio_configuration(dst+10,ri);
	return l+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_STATION_CFG,l);
}

static inline int cw_readelem_cisco_station_cfg(uint8_t *src,int len){
	
//	lw_readelem_
	return 0;
}

const char * cw_cisco_id_to_str(int elem_id);



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

#define cwmsg_addelem_cisco_certificate(cwmsg,crt,len)\
	(cwmsg)->pos+=cw_addelem_cisco_certificate(((cwmsg)->msgelems+(cwmsg)->pos),crt,len)

#define cwmsg_addelem_cisco_ap_regulatory_domain(cwmsg,radioinfo)\
	(cwmsg)->pos+=cw_addelem_cisco_ap_regulatory_domain(((cwmsg)->msgelems+(cwmsg)->pos),radioinfo)

#define cwmsg_addelem_cisco_station_cfg(cwmsg,radioinfo)\
	(cwmsg)->pos+=cw_addelem_cisco_station_cfg(((cwmsg)->msgelems+(cwmsg)->pos),radioinfo)


#endif
