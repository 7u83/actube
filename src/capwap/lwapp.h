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

#ifndef __LWAPP_H
#define __LWAPP_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "bstr.h"

/** 
 * @defgroup LWAPPConstants LWAPP Constats
 * @{
 */


/** LWAPP Version */
#define LW_VERSION 0

/** LWAPP Control Port */
#define LWAPP_CONTROL_PORT 12222
/** LWAPP Control Port as string */
#define LWAPP_CONTROL_PORT_STR "12222"

/** LWAPP Data Port */
#define LWAPP_DATA_PRT 12223

/** LWAPP Data Port as String */
#define LWAPP_DATA_PORT_STR "12223"

/** Block Size for Image Data */
#define LW_BLOCKSIZE_IMAGE_DATA 1024


/**@}*/


#define LW_VENDOR_ID_CISCO	4232704



/* macros to access transport header */
#define LWTH_GET_VERSION(th) (th[0]>>6)
#define LWTH_GET_L_FLAG(th) (th[0]&0x1)
#define LWTH_GET_F_FLAG(th) (th[0]&0x2)
#define LWTH_GET_C_FLAG(th) (th[0]&0x4)
#define LWTH_GET_RID(th) ((th[0]&0x38)>>3)
#define LWTH_GET_FRAGID(th) (th[1])
#define LWTH_GET_LENGTH(th) (ntohl(*((uint32_t*)(th)))&0xffff)

#define LWTH_SET_VERSION(th,v) (th[0] = (th[0]&0x3f) | (v<<6))
#define LWTH_SET_C_FLAG(th,v)  (th[0] = (th[0]&0xfB) | (v<<2))


#define LWMSG_GET_TYPE(m) (m[0])
#define LWMSG_GET_SEQNUM(m) (m[1])
#define LWMSG_GET_LEN(m) ( (ntohl(*((uint32_t*)(m)))&0xffff) )
#define LWMSG_GET_SESSIONID(m) ( ntohl(*  ( ( (uint32_t*)(m))[1])   ) )
#define LWMSG_GET_DATA(m) (m+8)


#define LWMSGELEM_GET_TYPE(m) (m[0])
#define LWMSGELEM_GET_LEN(m) (  (ntohl(*((uint32_t*)(m)))>>8)&0xffff )
#define LWMSGELEM_GET_DATA(m) (m+3)





//#define LWAPP_PACKET_PREAMBLE (CW_VERSION<<4)
//#define LWAPP_DTLS_PACKET_PREAMBLE (CW_VERSION<<4|1)



#define LW_MSG_DISCOVERY_REQUEST		1
#define LW_MSG_DISCOVERY_RESPONSE		2

#define LW_MSG_JOIN_REQUEST			3
#define LW_MSG_JOIN_RESPONSE			4

/*
                  Join ACK                             5
                  Join Confirm                         6
                  Unused                             7-9
                  Configure Request                   10
                  Configure Response                  11
                  Configuration Update Request        12
                  Configuration Update Response       13
                  WTP Event Request                   14
                  WTP Event Response                  15
                  Change State Event Request          16
                  Change State Event Response         17
                  Unused                           18-21
                  Echo Request                        22
                  Echo Response                       23
                  Image Data Request                  24
                  Image Data Response                 25
                  Reset Request                       26
                  Reset Response                      27
                  Unused                           28-29
                  Key Update Request                  30
                  Key Update Response                 31
                  Primary Discovery Request           32
*/


/* LWAPP message elements */

#define LW_ELEM_AC_ADDRESS				2
#define LW_ELEM_WTP_DESCRIPTOR 				3
#define LW_ELEM_WTP_NAME				5
#define LW_ELEM_AC_DESCRIPTOR				6
#define LW_ELEM_ADD_WLAN				7

#define LW_ELEM_MAC_OPERATION				11
#define LW_ELEM_TX_POWER				12
#define LW_ELEM_TX_POWER_LEVELS				13
#define LW_ELEM_DIRECT_SEQUENCE_CONTROL			14

#define LW_ELEM_80211_DELETE_WLAN			28

#define LW_ELEM_AC_NAME					31
#define LW_ELEM_LOCATION_DATA				35
#define LW_ELEM_STATISTICS_TIMER			37


#define LW_ELEM_RATE_SET				16
#define LW_ELEM_SUPPORTED_RATES				16

#define LW_ELEM_TEST					18

#define LW_ELEM_CERTIFICATE				44
#define LW_ELEM_WTP_BOARD_DATA				50
#define LW_ELEM_WTP_MODE_AND_TYPE			54
#define LW_ELEM_QOS					57

#define LW_ELEM_AC_IPV4_LIST				59
#define LW_ELEM_AP_IP_ADDR				82

#define LW_ELEM_VENDOR_SPECIFIC				104



/* LWAPP IEEE 802.11 bindings */

#define LW_ELEM_80211_ADD_WLAN				LW_ELEM_ADD_WLAN
#define LW_ELEM_80211_WTP_WLAN_RADIO_CONFIGURATION	8
#define LW_ELEM_80211_MULTI_DOMAIN_CAPABILITY		10
#define LW_ELEM_80211_MAC_OPERATION			11
#define LW_ELEM_80211_TX_POWER				LW_ELEM_TX_POWER 		/* 12 */
#define LW_ELEM_80211_TX_POWER_LEVELS			LW_ELEM_TX_POWER_LEVELS		/* 13 */
#define LW_ELEM_80211_DIRECT_SEQUENCE_CONTROL		LW_ELEM_DIRECT_SEQUENCE_CONTROL /* 14 */

#define LW_ELEM_80211_RATE_SET				LW_ELEM_RATE_SET		/* 16 */


#define LW_ELEM_80211_WTP_MODE_AND_TYPE			LW_ELEM_WTP_MODE_AND_TYPE	/* 54 */


/* useful macros and inline functions */

/** 
 * @defgroup LWAPPFunctions LWAPP Functions
 * @{
 */


#define lw_foreach_elem(d,msg,len) for(d=msg; d<msg+len; d=d+3+LWMSGELEM_GET_LEN(d))


#define lw_put_byte(dst,b) \
	(*(dst)=b)

#define lw_put_word(dst,w)\
	(*((uint16_t*)(dst)) = htons(w))

#define lw_put_dword(dst,dw)\
	(*((uint32_t*)(dst)) = htonl(dw))

#define lw_set_byte(dst,b) \
	(*(dst)=b);

#define lw_set_word(dst,b) \
	(*((uint16_t*)(dst)) = htons(w))

#define lw_set_dword(dst,dw)\
	(*((uint32_t*)(dst)) = htonl(dw))


#define lw_get_byte(src)\
	(*(uint8_t*)(src))

#define lw_get_word(src) \
	(ntohs(  *((uint16_t*)(src))))

#define lw_get_dword(src) \
	(ntohl(  *((uint32_t*)(src))))


/* the following functions are defined as static inline and not as 
   macro to avoid any side effects */


static inline int lw_put_data(uint8_t*dst,const uint8_t*data,uint16_t len)
{
	memcpy(dst,data,len); 
	return len;
}

static inline int lw_put_bstr(uint8_t * dst, const bstr_t b){
	lw_put_data(dst,bstr_data(b),bstr_len(b));
	return bstr_len(b);
}

static inline int lw_put_bstr16(uint8_t * dst, const bstr16_t b){
	lw_put_data(dst,bstr16_data(b),bstr16_len(b));
	return bstr16_len(b);
}



static inline int lw_put_str(uint8_t*dst,const uint8_t *str) {
	return lw_put_data(dst,str,strlen((char*)str));
}

static inline int lw_put_elem_hdr(uint8_t *dst,uint8_t type,uint16_t len)
{
	*dst=type;
	*((uint16_t*)(dst+1)) = htons(len);
	return 3;
}

/**@}*/


extern int lw_put_sockaddr(uint8_t *dst, struct sockaddr_storage *addr);


extern int lw_put_cisco_path_mtu(uint8_t *dst, uint16_t max, uint16_t padding);
#define lw_put_certificate(dst,cert,len) lw_put_data(dst,cert,len)
//extern int lw_put_ac_descriptor(uint8_t * dst, struct ac_info * acinfo);


/* function proto types */

extern uint16_t lw_checksum(uint8_t *d,int len);
//extern int lw_readelem_wtp_board_data(struct wtpinfo *wtpinfo, int type, uint8_t *msgelem, int len);
extern int lw_readelem_wtp_name(bstr_t * dst, int type, uint8_t * msgelem, int len);


//extern int lw_put_80211_wtp_wlan_radio_configuration(uint8_t*dst,struct radioinfo *ri);

extern const char * lw_vendor_id_to_str(uint32_t vendor_id);
extern const char * lw_elem_id_to_str(int elem_id);
extern const char * lw_msg_id_to_str(int msg_id);

extern int lw_put_image_data(uint8_t *dst,FILE *infile);


#endif
