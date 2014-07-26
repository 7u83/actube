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

#ifndef __CAPWAP_H
#define __CAPWAP_H

#include <stdint.h>
#include <arpa/inet.h>

#include "conn.h"

/* capwap version and iana number */
#define CW_VERSION 0
#define CWIANA_ENTERPRISE_NUMBER 0	/* for capwap base the number */


/* ports */
#define CAPWAP_CONTROL_PORT 5246
#define CAPWAP_CONTROL_PORT_STR "5246"




/* transport header flags */
#define CWTH_FLAGS_R1 0x01	/* bit 0 reserved 1 */
#define CWTH_FLAGS_R2 0x02	/* bit 1 reserved 2 */
#define CWTH_FLAGS_R3 0x04	/* bit 2 reserved 3 */
#define CWTH_FLAGS_K  0x08	/* bit 3 Keep alive flag */
#define CWTH_FLAGS_M  0x10	/* bit 4 MAC Adress field present*/
#define CWTH_FLAGS_W  0x20	/* bit 5 wireless info present */
#define CWTH_FLAGS_L  0x40	/* bit 6 last fragment */
#define CWTH_FLAGS_F  0x80	/* bit 7 fragment */
#define CWTH_FLAGS_T  0x100	/* bit 8 type of payload frame */

/* wireless binding ids */
#define CWTH_WBID_RESERVED1	0
#define CWTH_WBID_IEEE80211	1
#define CWTH_WBID_RESERVED2	2
#define CWTH_WBID_EPCGLOBAL	3




/* generic macroto to isolate bits from a dword */
#define CW_GET_DWORD_BITS(src,start,len) ((~(0xFFFFFFFF<<len)) & (src >> (32 - start - len)))

/* macros to acces transport header values */
#define CWTH_GET_PREAMBLE(th) (th[0]) 
//(ntohl(((uint32_t*)th)[0]) >> 24)
#define CWTH_GET_FRAGID(th) ((ntohl((((uint32_t*)th)[1]) >> 16) & 0xffff))
#define CWTH_GET_FRAGOFFSET(th) ((ntohl((((uint32_t*)th)[1]) >> 3) & 0x1fff))
#define CWTH_GET_RID(th) ((ntohl((((uint32_t*)th)[0]) >> 14) & 0x1f))
#define CWTH_GET_WBID(th) ((ntohl(((uint32_t*)th)[0]) >> 9) & 0x1f)
#define CWTH_GET_HLEN(th) ((ntohl(((uint32_t*)th)[0]) >> 19) & 0x1f)

#define CAPWAP_PACKET_PREAMBLE (CW_VERSION<<4)
#define CAPWAP_DTLS_PACKET_PREAMBLE (CW_VERSION<<4|1)

/*
 * control header stuff
 */ 
struct capwap_ctrlhdr
{
	int msgtype;
	int seqnum;
	int flags;
	uint8_t * msgelems;
	int msgelemslen;
};


//extern int capwap_parse_trnsprthdr(struct capwap_trnsprthdr * cwh,uint8_t *msg, int msglen);
//extern int capwap_parse_ctrlhdr(struct capwap_ctrlhdr * ch,uint8_t * msg, int len);

/* CAPWAP message types as defined in RFC 5416 */

#define	CWMSG_DISCOVERY_REQUEST			1 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_DISCOVERY_RESPONSE		2 + CWIANA_ENTERPRISE_NUMBER*256
#define	CWMSG_JOIN_REQUEST			3 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_JOIN_RESPONSE			4 + CWIANA_ENTERPRISE_NUMBER*256

/*	Configuration Status Request         5
           Configuration Status Response        6
           Configuration Update Request         7
           Configuration Update Response        8
           WTP Event Request                    9
           WTP Event Response                  10
           Change State Event Request          11
           Change State Event Response         12
*/	   
#define CWMSG_ECHO_REQUEST			13 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_ECHO_RESPONSE			14 + CWIANA_ENTERPRISE_NUMBER*256

/*          Image Data Request                  15
           Image Data Response                 16
*/

#define CWMSG_RESET_REQUEST			17 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_RESET_RESPONSE			18 + CWIANA_ENTERPRISE_NUMBER*256

/*
           Reset Request                       17
           Reset Response                      18
*/	   
#define CWMSG_PRIMARY_DISCOVERY_REQUEST		19 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_PRIMARY_DISCOVERY_RESPONSE	20 + CWIANA_ENTERPRISE_NUMBER*256

/*           Data Transfer Request               21
           Data Transfer Response              22
           Clear Configuration Request         23
           Clear Configuration Response        24
           Station Configuration Request       25
           Station Configuration Response      26
*/

#define CWMSG_MAXMSG				26 + CWIANA_ENTERPRISE_NUMBER*256


/*
* CAPWAP message elements as defined in  RFC 5416  
*/


#define CWMSGELEM_AC_DESCRIPTOR			1
/*   AC IPv4 List                                          2
   AC IPv6 List                                          3
*/   
#define CWMSGELEM_AC_NAME			4
/*
   AC Name with Priority                                 5
   AC Timestamp                                          6
   Add MAC ACL Entry                                     7
   Add Station                                           8
   Reserved                                              9
*/   
#define CWMSGELEM_CONTROL_IPV4_ADDRESS		10
#define CWMSGELEM_CONTROL_IPV6_ADDRESS		11
   
#define CWMSGELEM_CAPWAP_LOCAL_IPV4_ADDRESS	30
#define CWMSGELEM_CAPWAP_LOCAL_IPV6_ADDRESS	50
/*   CAPWAP Timers                                        12
   CAPWAP Transport Protocol                            51
   Data Transfer Data                                   13
   Data Transfer Mode                                   14
   Decryption Error Report                              15
   Decryption Error Report Period                       16
   Delete MAC ACL Entry                                 17
   Delete Station                                       18
   Reserved                                             19
*/   
#define CWMSGELEM_DISCOVERY_TYPE		20
/*   
   Duplicate IPv4 Address                               21
   Duplicate IPv6 Address                               22
*/   
#define CWMSGELEM_ECN_SUPPORT			53
 /*  Idle Timeout                                         23
   Image Data                                           24
   Image Identifier                                     25
   Image Information                                    26
   Initiate Download                                    27
   */
#define CWMSGELEM_LOCATION_DATA			28

#define CWMSGELEM_MAXIMUM_MESSAGE_LENGTH	29
#define CWMSGELEM_MTU_DISCOVERY_PADDING		52

/*   Radio Administrative State                           31
   Radio Operational State                              32
*/
#define CWMSGELEM_RESULT_CODE			33

/*   Returned Message Element                             34
*/   
#define CWMSGELEM_SESSION_ID			35
/*   Statistics Timer                                     36
   Vendor Specific Payload                              37
 */
#define CWMSGELEM_WTP_BOARD_DATA		38
   
#define CWMSGELEM_WTP_DESCRIPTOR		39
/*   WTP Fallback                                         40
  */
#define CWMSGELEM_WTP_FRAME_TUNNEL_MODE	41
/*   
   Reserved                                             42



*/
/*
   Reserved                                             43
*/   
#define CWMSGELEM_WTP_MAC_TYPE		44
#define CWMSGELEM_WTP_NAME		45
/*   
   Unused/Reserved                                      46
   WTP Radio Statistics                                 47
   WTP Reboot Statistics                                48
   WTP Static IP Address Information                    49
*/



/* wtp board data subelements */
#define CWBOARDDATA_MODELNO		0
#define CWBOARDDATA_SERIALNO		1
#define CWBOARDDATA_BOARDID		2
#define CWBOARDDATA_REVISION		3
#define CWBOARDDATA_MACADDRESS		4


/* */
#define CWACSECURITY_FLAGS_R		1	/* Reserved */
#define CWACSECURITY_FLAGS_S		2	/* DTLS psk */
#define CWACSECURITY_FLAGS_X		4	/* X.509 */


#define CWMSGSUBELEM_WTP_DESCRIPTOR_HARDWARE_VERSION		0
#define CWMSGSUBELEM_WTP_DESCRIPTOR_SOFTWARE_VERSION		1
#define CWMSGSUBELEM_WTP_DESCRIPTOR_BOOTLOADER_VERSION		2
#define CWMSGSUBELEM_WTP_DESCRIPTOR_OTHERSOFTWARE_VERSION	3

#include "wtpinfo.h"
#include "acinfo.h"

/* Frame tunnnel mode bits */
#define WTP_FRAME_TUNNEL_MODE_R	1	/* Reserved */
#define WTP_FRAME_TUNNEL_MODE_L	2	/* Local bridging */
#define WTP_FRAME_TUNNEL_MODE_E	4	/* 802.3 mode */
#define WTP_FRAME_TUNNEL_MODE_N	8	/* native mode */

#include "radioinfo.h"

#include "wtpinfo.h"


#define CW_DISCOVERY_TYPE_UNKNOWN	0
#define CW_DISCOVERY_TYPE_STATIC	1
#define CW_DISCOVERY_TYPE_DHCP		2
#define CW_DISCOVERY_TYPE_DNS		3
#define CW_DISCOVERY_TYPE_AC_REFERRAL	4



/* wtpinfo methods */
extern void wtpinfo_set_location(struct wtpinfo * wtpinfo, uint8_t * str, int len);
extern int wtpinfo_set_radioinfo(struct wtpinfo * wtpinfo,uint8_t *msgelem, int len);


#define CWRADIO_TYPE_B		1
#define CWRADIO_TYPE_A		2
#define CWRADIO_TYPE_G		4
#define CWRADIO_TYPE_N		8

/* wtp mac types */
#define WTP_MAC_TYPE_LOCAL	0
#define WTP_MAC_TYPE_SPLIT	1
#define WTP_MAC_TYPE_BOTH	2




//#define CWMSG_MAX_SIZE 65536
#define CWMSG_MAX_SIZE 2048

/* capwap timer default values */
#define CAPWAP_DISCOVERY_INTERVAL	5
#define CAPWAP_MAX_DISCOVERY_INTERVAL	20
#define CAPWAP_RETRANSMIT_INTERVAL	3
#define CAPWAP_MAX_DISCOVERIES		10
#define CAPWAP_MAX_RETRANSMIT		5
#define CAPWAP_SILENT_INTERVAL		30
#define CAPWAP_ECHO_INTERVAL 		30

#define CAPWAP_CIPHER			"PSK-AES128-CBC-SHA"


/* AC descriptor security flags */
#define AC_SECURITY_X			2	/* X.509 */
#define AC_SECURITY_S			4	/* PSK support */

/* AC dtls policy flags */
#define AC_DTLS_POLICY_C		2	/* Clear data channel support */
#define AC_DTLS_POLICY_D		4	/* DTLS Data channel support */		



extern void cwmsg_addelem_wtp_descriptor(struct cwmsg * cwmsg, struct wtpinfo * wtpinfo);
extern void cwmsg_addelem_ctrl_ip_addrs(struct cwmsg *msg, struct ac_info *acinfo);
extern void cwmsg_addelem_wtp_board_data(struct cwmsg * cwmsg, struct wtpinfo * wtpinfo);
extern void cwmsg_addelem_cw_local_ip_addr(struct cwmsg *msg, struct conn * conn);
extern void cwmsg_addelem_wtp_radio_infos(struct cwmsg * cwmsg,struct wtpinfo * wtpinfo);
extern void cwmsg_addelem_result_code(struct cwmsg *msg,int rc);

//extern void cwsend_discovery_reponse(struct conn * conn, struct ac_info * acinfo);
//extern int process_msgelems(uint8_t * msgelems,  int len,
//		int (*callback)(void*,int,uint8_t*,int),void *arg );




//void cwsend_discovery_response(struct conn * conn,int seqnum, struct radioinfo * radioinfo,  struct ac_info * acinfo, struct wtpinfo * wtpinfo);
//
extern void cwsend_discovery_response(struct conn * conn,int seqnum, struct radioinfo * radioinfo,  struct ac_info * acinfo, struct wtpinfo * wtpinfo);
extern int cwsend_discovery_request(struct conn * conn,struct radioinfo * radioinfo,struct wtpinfo * wtpinfo);
extern void cwsend_join_response(struct conn * conn,int seqnum, int rc, struct radioinfo * radioinfo, struct ac_info * acinfo, struct wtpinfo * wtpinfo);


extern void process_discovery_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len);
extern void process_join_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len);
void cwread_discovery_response(struct ac_info * acinfo, uint8_t * msg, int len);
extern int cwsend_echo_response(struct conn * conn,int seqnum,struct radioinfo * radioinfo); //,struct wtpinfo * wtpinfo


#endif
