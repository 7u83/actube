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


#define CWTH_GET_FLAG_R1(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_R1 ) ? 1:0)
#define CWTH_GET_FLAG_R2(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_R2 ) ? 1:0)
#define CWTH_GET_FLAG_R3(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_R3 ) ? 1:0)

#define CWTH_GET_FLAG_K(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_K ) ? 1:0)
#define CWTH_GET_FLAG_M(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_M ) ? 1:0)
#define CWTH_GET_FLAG_W(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_W ) ? 1:0)
#define CWTH_GET_FLAG_L(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_L ) ? 1:0)
#define CWTH_GET_FLAG_F(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_F ) ? 1:0)
#define CWTH_GET_FLAG_T(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_T ) ? 1:0)

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

/*
struct capwap_ctrlhdr
{
	int msgtype;
	int seqnum;
	int flags;
	uint8_t * msgelems;
	int msgelemslen;
};
*/

//extern int capwap_parse_trnsprthdr(struct capwap_trnsprthdr * cwh,uint8_t *msg, int msglen);
//extern int capwap_parse_ctrlhdr(struct capwap_ctrlhdr * ch,uint8_t * msg, int len);

/* CAPWAP message types as defined in RFC 5416 */

#define	CWMSG_DISCOVERY_REQUEST			1 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_DISCOVERY_RESPONSE		2 + CWIANA_ENTERPRISE_NUMBER*256
#define	CWMSG_JOIN_REQUEST			3 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_JOIN_RESPONSE			4 + CWIANA_ENTERPRISE_NUMBER*256

#define CWMSG_CONFIGURATION_STATUS_REQUEST	5 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_CONFIGURATION_STATUS_RESPONSE	6 + CWIANA_ENTERPRISE_NUMBER*256

#define CWMSG_CONFIGURATION_UPDATE_REQUEST	7
#define CWMSG_CONFIGURATION_UPDATE_RESPONSE	8

#define CWMSG_WTP_EVENT_REQUEST			9
#define CWMSG_WTP_EVENT_RESPONSE		10

#define CWMSG_CHANGE_STATE_EVENT_REQUEST	11
#define CWMSG_CHANGE_STATE_EVENT_RESPONSE	12

#define CWMSG_ECHO_REQUEST			13 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_ECHO_RESPONSE			14 + CWIANA_ENTERPRISE_NUMBER*256

#define CWMSG_IMAGE_DATA_REQUEST		15 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_IMAGE_DATA_RESPONSE		16 + CWIANA_ENTERPRISE_NUMBER*256

#define CWMSG_RESET_REQUEST			17 + CWIANA_ENTERPRISE_NUMBER*256
#define CWMSG_RESET_RESPONSE			18 + CWIANA_ENTERPRISE_NUMBER*256

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
*/

#define CWMSGELEM_AC_TIMESTAMP			6

/*   Add MAC ACL Entry                                     7
   Add Station                                           8
   Reserved                                              9
*/   
#define CWMSGELEM_CONTROL_IPV4_ADDRESS		10
#define CWMSGELEM_CONTROL_IPV6_ADDRESS		11
   
#define CWMSGELEM_CAPWAP_LOCAL_IPV4_ADDRESS	30
#define CWMSGELEM_CAPWAP_LOCAL_IPV6_ADDRESS	50
#define CWMSGELEM_CAPWAP_TIMERS			12
/*   CAPWAP Transport Protocol                            51
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
*/
#define CWMSGELEM_IMAGE_DATA			24

#define CWMSGELEM_IMAGE_IDENTIFIER		25
  /* Image Information                                    26
   Initiate Download                                    27
   */
#define CWMSGELEM_LOCATION_DATA			28

#define CWMSGELEM_MAXIMUM_MESSAGE_LENGTH	29
#define CWMSGELEM_MTU_DISCOVERY_PADDING		52

#define CWMSGELEM_RADIO_ADMINISTRATIVE_STATE	31
#define CWMSGELEM_RADIO_OPERATIONAL_STATE	32

#define CWMSGELEM_RESULT_CODE			33

/*   Returned Message Element                             34
*/   
#define CWMSGELEM_SESSION_ID			35
#define CWMSGELEM_STATISTICS_TIMER		36
#define CWMSGELEM_VENDOR_SPECIFIC_PAYLOAD	37

#define CWMSGELEM_WTP_BOARD_DATA		38
   
#define CWMSGELEM_WTP_DESCRIPTOR		39
/*   WTP Fallback                                         40
  */
#define CWMSGELEM_WTP_FRAME_TUNNEL_MODE		41
#define CWMSGELEM_RESERVED_1			42

/*
   Reserved                                             43
*/   
#define CWMSGELEM_WTP_MAC_TYPE			44
#define CWMSGELEM_WTP_NAME			45
/*   
   Unused/Reserved                                      46
   WTP Radio Statistics                                 47
*/
#define CWMSGELEM_WTP_REBOOT_STATISTICS		48
/*   WTP Static IP Address Information                    49
*/



/* wtp board data subelements */
#define CWBOARDDATA_MODELNO		0
#define CWBOARDDATA_SERIALNO		1
#define CWBOARDDATA_BOARDID		2
#define CWBOARDDATA_REVISION		3
#define CWBOARDDATA_MACADDRESS		4


/* */
#define CWACSECURITY_FLAGS_R		1	/* Reserved */
#define CWACSECURITY_FLAGS_X		2	/* X.509 */
#define CWACSECURITY_FLAGS_S		4	/* DTLS psk */


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
#define CAPWAP_WAIT_DTLS		60
#define	CAPWAP_WAIT_JOIN		60


//#define CAPWAP_CIPHER			"PSK-AES128-CBC-SHA:"
//#define CAPWAP_CIPHER	"AES128-SHA"
#define CAPWAP_CIPHER	"ALL"




/* AC descriptor security flags */
#define AC_SECURITY_X			2	/* X.509 */
#define AC_SECURITY_S			4	/* PSK support */

/* AC dtls policy flags */
#define AC_DTLS_POLICY_C		2	/* Clear data channel support */
#define AC_DTLS_POLICY_D		4	/* DTLS Data channel support */		


struct image_data{
	uint8_t * data;
	uint8_t type;
	int len;
};

struct image_identifier{
	uint32_t vendor_id;
	char *name;
};





extern void cwmsg_addelem_wtp_descriptor(struct cwmsg * cwmsg, struct wtpinfo * wtpinfo);
extern void cwmsg_addelem_ctrl_ip_addrs(struct cwmsg *msg, struct ac_info *acinfo);
extern void cwmsg_addelem_wtp_board_data(struct cwmsg * cwmsg, struct wtpinfo * wtpinfo);
extern void cwmsg_addelem_cw_local_ip_addr(struct cwmsg *msg, struct conn * conn);

//extern void cwmsg_addelem_wtp_radio_infos(struct cwmsg * cwmsg,struct wtpinfo * wtpinfo);
extern void cwmsg_addelem_wtp_radio_infos(struct cwmsg * msg,struct radioinfo * radioinfos);

extern void cwmsg_addelem_result_code(struct cwmsg *msg,int rc);
extern void cwmsg_addelem_vendor_specific_payload(struct cwmsg *msg,int vendor_id, int type, uint8_t * payload,int len);

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
extern void process_conf_status_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len);

extern void cwread_discovery_response(struct ac_info * acinfo, uint8_t * msg, int len);
extern int cwsend_echo_response(struct conn * conn,int seqnum,struct radioinfo * radioinfo); //,struct wtpinfo * wtpinfo
extern void cwread_image_data_request(struct ac_info * acinfo, uint8_t * msg, int len);
extern void cwsend_image_data_response(struct conn * conn,int seqnum, int rc);
extern int cwsend_image_data_request(struct conn * conn, struct image_data * data, struct image_identifier *id );
extern int cwread_change_state_event_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len);
extern void cwsend_change_state_event_response(struct conn * conn,int seqnum, struct radioinfo * radioinfo);
extern int cwread_wtp_event_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len);
extern void cwread_configuration_status_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len);
extern void cwsend_conf_status_response(struct conn * conn,int seqnum, int rc, struct radioinfo * radioinfo, struct ac_info * acinfo, struct wtpinfo * wtpinfo);

extern void cwsend_unknown_response(struct conn * conn,int seqnum, int unknow_request);

extern const char * cw_msgelemtostr(int elem);
extern const char * cw_msgtostr(int type);

extern const char * cw_ianavendoridtostr(int id);
extern int hdr_print(char *str, uint8_t *packet, int len);

extern int cw_readelem_ecn_support(uint8_t *ecn_support, int type, uint8_t * msgelem, int len);
extern int cw_readelem_ac_name(uint8_t **dst, int type,uint8_t *msgelem, int len);
extern int cw_readelem_wtp_reboot_statistics(struct wtp_reboot_statistics *s, int type,uint8_t *msgelem, int len);
extern int cw_readelem_cw_local_ip_addr(struct sockaddr * local_ip, int type, uint8_t * msgelem, int len);
extern int cw_readelem_radio_administrative_state(struct radioinfo * radioinfo, int type,uint8_t *msgelem, int len);
extern int cw_readelem_radio_operational_state(struct radioinfo * radioinfo, int type,uint8_t *msgelem, int len);

extern int cw_readelem_statistics_timer(uint16_t *timer, int type, uint8_t * msgelem, int len);
extern int cw_readelem_result_code(uint32_t *result_code, int type, uint8_t * msgelem, int len);




#define CW_VENDOR_ID_FSF	11591
#define CW_VENDOR_ID_ZYXEL	890
#define CW_VENDOR_ID_CISCO	4232704



#define CW_RESULT_SUCCESS				0
#define CW_RESULT_FAILURE				1
#define CW_RESULT_SUCCESS_NAT				2
#define CW_RESULT_JOIN_FAILURE				3
#define CW_RESULT_JOIN_RESOURCE_DEPLETION		4
#define CW_RESULT_JOIN_UNKNOWN_SOURCE			5

/*
     6  Join Failure (Incorrect Data)

      7  Join Failure (Session ID Already in Use)

      8  Join Failure (WTP Hardware Not Supported)

      9  Join Failure (Binding Not Supported)

      10 Reset Failure (Unable to Reset)

      11 Reset Failure (Firmware Write Error)

      12 Configuration Failure (Unable to Apply Requested Configuration
         - Service Provided Anyhow)

      13 Configuration Failure (Unable to Apply Requested Configuration
         - Service Not Provided)

      14 Image Data Error (Invalid Checksum)

      15 Image Data Error (Invalid Data Length)

      16 Image Data Error (Other Error)

      17 Image Data Error (Image Already Present)

      18 Message Unexpected (Invalid in Current State)

      19 Message Unexpected (Unrecognized Request)

      20 Failure - Missing Mandatory Message Element

      21 Failure - Unrecognized Message Element

      22 Data Transfer Error (No Information to Transfer)

*/

#endif
