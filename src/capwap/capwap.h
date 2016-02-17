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
 * @brief CAPWAP desfinitions 
 */



#ifndef __CAPWAP_H
#define __CAPWAP_H


#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <errno.h>
#include <arpa/inet.h>

#include "conn.h"

#include "lwapp.h"
#include "strlist.h"



/* capwap version and iana number */

/**
 *@defgroup CAPWAPConstants CAPWAP Constants 
 *@{
 */

/** CAPWAP Version */
#define CAPWAP_VERSION ((uint8_t)0)

#define CWIANA_ENTERPRISE_NUMBER 0

/** CAPWAP Control Port*/
#define CAPWAP_CONTROL_PORT 5246
/** CAPWAP Control Port as String */
#define CAPWAP_CONTROL_PORT_STR "5246"

/** CAPWAP Data Port */
#define CAPWAP_DATA_PPORT 5247

/** CAWPAP DataPort as String */
#define CAPWAP_DATA_PPORT_STR "5247"



/**
 * CAWAP States
 */
enum capwap_states {
	CW_STATE_NONE = 0,
	/** Discovery State */
	CW_STATE_DISCOVERY,
	/** Join State */
	CW_STATE_JOIN,
	/** Config State */
	CW_STATE_CONFIGURE,
	/** Image Data Upload */
	CW_STATE_IMAGE_DATA,
	CW_STATE_UPDATE,
	/** Run State */
	CW_STATE_RUN
};

/**@}*/


/* transport header flags */
#define CWTH_FLAGS_R1 0x01	/* bit 0 reserved 1 */
#define CWTH_FLAGS_R2 0x02	/* bit 1 reserved 2 */
#define CWTH_FLAGS_R3 0x04	/* bit 2 reserved 3 */
#define CWTH_FLAGS_K  0x08	/* bit 3 Keep alive flag */
#define CWTH_FLAGS_M  0x10	/* bit 4 MAC Adress field present */
#define CWTH_FLAGS_W  0x20	/* bit 5 wireless info present */
#define CWTH_FLAGS_L  0x40	/* bit 6 last fragment */
#define CWTH_FLAGS_F  0x80	/* bit 7 fragment */
#define CWTH_FLAGS_T  0x100	/* bit 8 type of payload frame */


/**
 * CAWAP header flags.
 */
#define CW_FLAG_HDR_R1 0x01	/* bit 0 reserved 1 */
#define CW_FLAG_HDR_R2 0x02	/* bit 1 reserved 2 */
#define CW_FLAG_HDR_R3 0x04	/* bit 2 reserved 3 */
#define CW_FLAG_HDR_K  0x08	/* bit 3 Keep alive flag */
#define CW_FLAG_HDR_M  0x10	/* bit 4 MAC Adress field present */
#define CW_FLAG_HDR_W  0x20	/* bit 5 wireless info present */
#define CW_FLAG_HDR_L  0x40	/* bit 6 last fragment */
#define CW_FLAG_HDR_F  0x80	/* bit 7 fragment */
#define CW_FLAG_HDR_T  0x100	/* bit 8 type of payload frame */



/* wireless binding ids */
#define CWTH_WBID_RESERVED1	0
#define CWTH_WBID_IEEE80211	1
#define CWTH_WBID_RESERVED2	2
#define CWTH_WBID_EPCGLOBAL	3


#define CAPWAP_PACKET_PREAMBLE (CAPWAP_VERSION<<4)
#define CAPWAP_DTLS_PACKET_PREAMBLE (CAPWAP_VERSION<<4|1)

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

/** 
 *@addtogroup CAPWAPConstants 
 *@{
 */

/**
 * CAPWAP message types as defined in RFC 5416 
 */
enum cw_message_types{

/**Discovery Request = 1*/
CW_MSG_DISCOVERY_REQUEST = 			1,
/** Discovery Response = 2 */
CW_MSG_DISCOVERY_RESPONSE = 			2,
/** Join Request = 3 */
CW_MSG_JOIN_REQUEST = 				3,
/** Join Response = 4 */
CW_MSG_JOIN_RESPONSE = 				4,
/** Config. Status Request = 5*/
CW_MSG_CONFIGURATION_STATUS_REQUEST = 		5,
/** Config. Status Response = 6 */
CW_MSG_CONFIGURATION_STATUS_RESPONSE = 		6,

CW_MSG_CONFIGURATION_UPDATE_REQUEST = 		7,
CW_MSG_CONFIGURATION_UPDATE_RESPONSE = 		8,

CW_MSG_WTP_EVENT_REQUEST = 			9,
CW_MSG_WTP_EVENT_RESPONSE = 			10,

CW_MSG_CHANGE_STATE_EVENT_REQUEST = 		11,
CW_MSG_CHANGE_STATE_EVENT_RESPONSE = 		12,

CW_MSG_ECHO_REQUEST = 				13,
CW_MSG_ECHO_RESPONSE = 				14,

CW_MSG_IMAGE_DATA_REQUEST = 			15,
CW_MSG_IMAGE_DATA_RESPONSE = 			16,

CW_MSG_RESET_REQUEST = 				17,
CW_MSG_RESET_RESPONSE = 			18,

CW_MSG_PRIMARY_DISCOVERY_REQUEST = 		19,
CW_MSG_PRIMARY_DISCOVERY_RESPONSE = 		20,

CW_MSG_DATA_TRANSFER_REQUEST = 			21,
CW_MSG_DATA_TRANSFER_RESPONSE = 		22,

CW_MSG_CLEAR_CONFIGURATION_REQUEST = 		23,
CW_MSG_CLEAR_CONFIGURATION_RESPONSE = 		24,

CW_STATION_CONFIGURATION_REQUEST = 		25,
CW_STATION_CONFIGURATION_RESPONSE = 		26,

CW_MSG_MAXMSG	=				26

};

//cw_msg_t;

/**@}*/

 

/*
#define	CW_MSG_DISCOVERY_REQUEST			1
#define CW_MSG_DISCOVERY_RESPONSE			2
#define	CW_MSG_JOIN_REQUEST				3
#define CW_MSG_JOIN_RESPONSE				4

#define CW_MSG_CONFIGURATION_STATUS_REQUEST		5
#define CW_MSG_CONFIGURATION_STATUS_RESPONSE		6

#define CW_MSG_CONFIGURATION_UPDATE_REQUEST		7
#define CW_MSG_CONFIGURATION_UPDATE_RESPONSE		8

#define CW_MSG_WTP_EVENT_REQUEST			9
#define CW_MSG_WTP_EVENT_RESPONSE			10

#define CW_MSG_CHANGE_STATE_EVENT_REQUEST		11
#define CW_MSG_CHANGE_STATE_EVENT_RESPONSE		12

#define CW_MSG_ECHO_REQUEST				13
#define CW_MSG_ECHO_RESPONSE				14

#define CW_MSG_IMAGE_DATA_REQUEST			15
#define CW_MSG_IMAGE_DATA_RESPONSE			16

#define CW_MSG_RESET_REQUEST				17
#define CW_MSG_RESET_RESPONSE				18

#define CW_MSG_PRIMARY_DISCOVERY_REQUEST		19
#define CW_MSG_PRIMARY_DISCOVERY_RESPONSE		20

#define CW_MSG_DATA_TRANSFER_REQUEST			21
#define CW_MSG_DATA_TRANSFER_RESPONSE			22

#define CW_MSG_CLEAR_CONFIGURATION_REQUEST		23
#define CW_MSG_CLEAR_CONFIGURATION_RESPONSE		24

#define CW_STATION_CONFIGURATION_REQUEST		25
#define CW_STATION_CONFIGURATION_RESPONSE		26

#define CW_MSG_MAXMSG					26
*/

/**
* CAPWAP message elements as defined in  RFC 5415
*/


#define CW_ELEM_AC_DESCRIPTOR				1
#define CW_ELEM_AC_IPV4_LIST				2
#define CW_ELEM_AC_IPV6_LIST				3
#define CW_ELEM_AC_NAME					4
#define CW_ELEM_AC_NAME_WITH_PRIORITY			5
#define CW_ELEM_AC_NAME_WITH_INDEX	CW_ELEM_AC_NAME_WITH_PRIORITY	/* Draft 7 naming */
#define CW_ELEM_AC_TIMESTAMP				6
#define CW_ELEM_ADD_MAC_ACL_ENTRY			7
#define CW_ELEM_ADD_STATION				8
#define CW_ELEM_RESERVED_9				9
#define CW_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS		10
#define CW_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS		11
#define CW_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS		30
#define CW_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS		50
#define CW_ELEM_CAPWAP_TIMERS				12
#define CW_ELEM_CAPWAP_TRANSPORT_PROTOCOL		51	/* not in draft 7 */
#define CW_ELEM_DATA_TRANSFER_DATA			13
#define CW_ELEM_DATA_TRANSFER_MODE			14
#define CW_ELEM_DECRYPTION_ERROR_REPORT			15
#define CW_ELEM_DECRYPTION_ERROR_REPORT_PERIOD		16
#define CW_ELEM_DELETE_MAC_ACL_ENTRY			17
#define CW_ELEM_DELETE_STATION				18
#define CW_ELEM_RESERVED_19				19
#define CW_ELEM_DISCOVERY_TYPE				20
#define CW_ELEM_DUPLICATE_IPV4_ADDRESS			21
#define CW_ELEM_DUPLICATE_IPV6_ADRESS			22
#define CWMSGELEM_ECN_SUPPORT				53
#define CW_ELEM_IDLE_TIMEOUT				23
#define CW_ELEM_IMAGE_DATA				24
#define CW_ELEM_IMAGE_IDENTIFIER			25
#define CW_ELEM_IMAGE_INFORMATION			26
#define CW_ELEM_INITIATE_DOWNLOAD			27
#define CW_ELEM_LOCATION_DATA				28
#define CW_ELEM_MAXIMUM_MESSAGE_LENGTH			29
#define CWMSGELEM_MTU_DISCOVERY_PADDING			52
#define CW_ELEM_RADIO_ADMINISTRATIVE_STATE		31
#define CW_ELEM_RADIO_OPERATIONAL_STATE			32
#define CW_ELEM_RESULT_CODE				33
#define CW_ELEM_RETURNED_MESSAGE_ELEMENT		34
#define CW_ELEM_SESSION_ID				35
#define CW_ELEM_STATISTICS_TIMER			36
#define CW_ELEM_VENDOR_SPECIFIC_PAYLOAD			37
#define CW_ELEM_WTP_BOARD_DATA				38
#define CW_ELEM_WTP_DESCRIPTOR				39
#define CW_ELEM_WTP_FALLBACK				40
#define CW_ELEM_WTP_FRAME_TUNNEL_MODE			41
#define CW_ELEM_RESERVED_42				42
#define CW_ELEM_RESERVED_43				43
#define CW_ELEM_WTP_MAC_TYPE				44
#define CW_ELEM_WTP_NAME				45
#define CW_ELEM_RESERVED_46				46
#define CW_ELEM_WTP_RADIO_STATISTICS			47
#define CW_ELEM_WTP_REBOOT_STATISTICS			48
#define CW_ELEM_WTP_STATIC_IP_ADDRESS_INFORMATION	49
#define CW_ELEM_WTP_STATIC_IP_ADDR_INFO			49


/* Cisco's CAPWAP definitions (CAPWAP draft 7) */
#define	CW_ELEM_WTP_IPV4_IP_ADDRESS			42
#define	CW_ELEM_WTP_IPV6_IP_ADDRESS			43


/* pseudo message elements, defined for libcapwap */

#define XCWMSGELEM_CAPWAP_LOCAL_IP_ADDRESS	0x10000	/* means LOCAL_IPV4 or IPV6 ADRESS */
#define XCWMSGELEM_CAPWAP_CONTROL_IP_ADDRESS	0x20000	/* means LOCAL_IPV4 or IPV6 ADRESS */
#define XCWMSGELEM_CAPWAP_RADIO_INFO		0x30000	/* a radio info element  */




/* wtp board data subelements */
#define CW_BOARDDATA_MODELNO		0
#define CW_BOARDDATA_SERIALNO		1
#define CW_BOARDDATA_BOARDID		2
#define CW_BOARDDATA_REVISION		3
#define CW_BOARDDATA_MACADDRESS		4


/* AC Security flags for authentication  */
#define CW_FLAG_AC_SECURITY_R		1	/* Reserved */
#define CW_FLAG_AC_SECURITY_X		2	/* X.509 */
#define CW_FLAG_AC_SECURITY_S		4	/* DTLS psk */


#define CW_FLAG_RMAC_RESERVED		0
#define CW_FLAG_RMAC_SUPPORTED		1
#define CW_FLAG_RMAC_NOT_SUPPORTED	2


#define CW_SUBELEM_WTP_HARDWARE_VERSION			0
#define CW_SUBELEM_WTP_SOFTWARE_VERSION			1
#define CW_SUBELEM_WTP_BOOTLOADER_VERSION		2
#define CW_SUBELEM_WTP_OTHERSOFTWARE_VERSION		3

#define CW_SUBELEM_AC_HARDWARE_VERSION			4
#define CW_SUBELEM_AC_SOFTWARE_VERSION			5


//#include "wtpinfo.h"
//#include "acinfo.h"

/* Frame tunnnel mode bits */
#define WTP_FRAME_TUNNEL_MODE_R	1	/* Reserved */
#define WTP_FRAME_TUNNEL_MODE_L	2	/* Local bridging */
#define WTP_FRAME_TUNNEL_MODE_E	4	/* 802.3 mode */
#define WTP_FRAME_TUNNEL_MODE_N	8	/* native mode */

#include "radioinfo.h"

//#include "wtpinfo.h"


#define CW_DISCOVERY_TYPE_UNKNOWN	0
#define CW_DISCOVERY_TYPE_STATIC	1
#define CW_DISCOVERY_TYPE_DHCP		2
#define CW_DISCOVERY_TYPE_DNS		3
#define CW_DISCOVERY_TYPE_AC_REFERRAL	4



/* wtpinfo methods */
//extern void wtpinfo_set_location(struct wtpinfo *wtpinfo, uint8_t * str, int len);
//extern int wtpinfo_set_radioinfo(struct wtpinfo *wtpinfo, uint8_t * msgelem, int len);


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

#define CAPWAP_TIMERS			((CAPWAP_DISCOVERY_INTERVAL << 8) | CAPWAP_ECHO_INTERVAL)


//#define CAPWAP_CIPHER                 "PSK-AES128-CBC-SHA:"
//#define CAPWAP_CIPHER "AES128-SHA"

#ifdef WITH_GNUTLS
#define CAPWAP_CIPHER	"NORMAL"
#else
#define CAPWAP_CIPHER	"ALL"
#endif




/* AC descriptor security flags */
#define AC_SECURITY_X			2	/* X.509 */
#define AC_SECURITY_S			4	/* PSK support */

/* AC dtls policy flags */
#define CW_FLAG_DTLS_POLICY_R		1	/* Clear data channel support */
#define CW_FLAG_DTLS_POLICY_C		2	/* Clear data channel support */
#define CW_FLAG_DTLS_POLICY_D		4	/* DTLS Data channel support */



struct cwimage_data {
	uint8_t *data;
	uint8_t type;
	int len;
	uint32_t vendor_id;
	uint8_t *identifier;
};

/*
struct cwimage_identifier{
	uint32_t vendor_id;
	char *name;
};
*/



/*
extern void cwmsg_addelem_wtp_descriptor(struct cwmsg *cwmsg, struct wtpinfo *wtpinfo);
extern void cwmsg_addelem_ctrl_ip_addrs(struct cwmsg *msg, struct ac_info *acinfo);
extern void cwmsg_addelem_wtp_board_data(struct cwmsg *cwmsg, struct wtpinfo *wtpinfo);
extern void cwmsg_addelem_cw_local_ip_addr(struct cwmsg *msg, struct conn *conn);
*/

//extern void cwmsg_addelem_wtp_radio_infos(struct cwmsg * cwmsg,struct wtpinfo * wtpinfo);
extern void cwmsg_addelem_wtp_radio_infos(struct cwmsg *msg,
					  struct radioinfo *radioinfos);

//extern void cwmsg_addelem_result_code(struct cwmsg *msg, int rc);
extern void cwmsg_addelem_vendor_specific_payload(struct cwmsg *msg, int vendor_id,
						  int type, uint8_t * payload, int len);

//extern void cwsend_discovery_reponse(struct conn * conn, struct ac_info * acinfo);
//extern int process_msgelems(uint8_t * msgelems,  int len,
//              int (*callback)(void*,int,uint8_t*,int),void *arg );




//void cwsend_discovery_response(struct conn * conn,int seqnum, struct radioinfo * radioinfo,  struct ac_info * acinfo, struct wtpinfo * wtpinfo);
//

/*
extern void cwsend_discovery_response(struct conn *conn, int seqnum,
				      struct radioinfo *radioinfo, struct ac_info *acinfo,
				      struct wtpinfo *wtpinfo);
extern int cwsend_discovery_request(struct conn *conn, struct radioinfo *radioinfo,
				    struct wtpinfo *wtpinfo);
extern void cwsend_join_response(struct conn *conn, int seqnum, int rc,
				 struct radioinfo *radioinfo, struct ac_info *acinfo,
				 struct wtpinfo *wtpinfo);

*/

//extern void cwread_discovery_request(struct wtpinfo *wtpinfo, uint8_t * msg, int len);
//extern void process_join_request(struct wtpinfo *wtpinfo, uint8_t * msg, int len);
//extern void process_conf_status_request(struct wtpinfo *wtpinfo, uint8_t * msg, int len);

/*
extern void cwread_discovery_response(struct ac_info *acinfo, uint8_t * msg, int len);
extern void cwsend_image_data_response(struct conn *conn, int seqnum, int rc);
extern int cwsend_image_data_request(struct conn *conn, struct cwimage_data *data,
				     struct image_identifier *id);
extern int cwread_change_state_event_request(struct wtpinfo *wtpinfo, uint8_t * msg,
					     int len);
extern void cwsend_change_state_event_response(struct conn *conn, int seqnum,
					       struct radioinfo *radioinfo);
extern int cwread_wtp_event_request(struct wtpinfo *wtpinfo, uint8_t * msg, int len);
extern void cwread_configuration_status_request(struct wtpinfo *wtpinfo, uint8_t * msg,
						int len);
extern void cwsend_conf_status_response(struct conn *conn, int seqnum, int rc,
					struct radioinfo *radioinfo,
					struct ac_info *acinfo, struct wtpinfo *wtpinfo);

extern void cwsend_unknown_response(struct conn *conn, int seqnum, int unknow_request);

*/

extern int hdr_print(char *str, uint8_t * packet, int len);

extern int cw_readelem_ecn_support(uint8_t * ecn_support, int type, uint8_t * msgelem,
				   int len);
extern int cw_readelem_maximum_message_length(uint16_t * dst, int type, uint8_t * msgelem,
					      int len);
extern int cw_readelem_ac_name(uint8_t ** dst, int type, uint8_t * msgelem, int len);
//extern int cw_readelem_wtp_reboot_statistics(struct wtp_reboot_statistics *s, int type,
//					     uint8_t * msgelem, int len);
extern int cw_readelem_cw_local_ip_addr(struct sockaddr *local_ip, int type,
					uint8_t * msgelem, int len);
//extern int cw_readelem_radio_administrative_state(struct radioinfo *radioinfo, int type,
//						  uint8_t * msgelem, int len);
//extern int cw_readelem_radio_operational_state(struct radioinfo *radioinfo, int type,
//					       uint8_t * msgelem, int len);

extern int cw_readelem_statistics_timer(uint16_t * timer, int type, uint8_t * msgelem,
					int len);
extern int cw_readelem_result_code(uint32_t * result_code, int type, uint8_t * msgelem,
				   int len);
extern int cw_readelem_mtu_discovery_padding(int type, uint8_t * msgelem, int len);
extern int cw_readelem_vendor_specific_payload(void *data, int msgtype, int elemtype,
					       uint8_t * msgelem, int len);




#define CW_VENDOR_ID_FSF	11591
#define CW_VENDOR_ID_ZYXEL	890
#define CW_VENDOR_ID_CISCO	LW_VENDOR_ID_CISCO
#define CW_VENDOR_ID_LANCOM	2356
#define CW_VENDOR_ID_FORTINET	12356



#define CW_RESULT_SUCCESS					0
#define CW_RESULT_MISSING_AC_LIST				1
#define CW_RESULT_SUCCESS_NAT					2
#define CW_RESULT_JOIN_FAILURE					3
#define CW_RESULT_JOIN_RESOURCE_DEPLETION			4
#define CW_RESULT_JOIN_UNKNOWN_SOURCE				5
#define CW_RESULT_JOIN_INCORRECT_DATA				6
#define CW_RESULT_JOIN_FAILURE_SESSION_ALREADY_IN_USE		7
#define CW_RESULT_JOIN_FAILURE_WTP_HARDWARE_NOT_SUPPORTED	8
#define CW_RESULT_JOIN_FAILURE_BINDING_NOT_SUPPORTED		9
#define CW_RESULT_RESET_UNABLE_TO_RESET				10

#define CW_RESULT_RESET_FAILURE_FIRMWARE_WRITE_ERROR		11

#define CW_RESULT_CONFIGURATION_FAILURE				12	//Configuration Failure (Unable to Apply Requested Configuration

//         - Service Provided Anyhow)
/*

      13 Configuration Failure (Unable to Apply Requested Configuration
         - Service Not Provided)
*/
#define CW_RESULT_IMAGE_DATA_INVALID_CHECKSUM			14 //Image Data Error (Invalid Checksum)


#define CW_RESULT_IMAGE_DATA_IVALID_LENGTH			15	//Image Data Error (Invalid Data Length)

#define CW_RESULT_IMAGE_DATA_ERROR				16	//Image Data Error (Other Error)
/*
      17 Image Data Error (Image Already Present)
*/

#define CW_RESULT_MSG_INVALID_IN_CURRENT_STATE			18
#define CW_RESULT_MSG_UNRECOGNIZED				19
#define CW_RESULT_MISSING_MAND_ELEM				20

/*
      21 Failure - Unrecognized Message Element

      22 Data Transfer Error (No Information to Transfer)

*/



extern void cw_read_image_data_request(struct cwimage_data *, uint8_t * msg, int len);

//extern int cw_readelem_ac_descriptor(struct ac_info *acinfo, int type, uint8_t * msgelem,
//				     int len);
extern int cw_readelem_capwap_local_ip_addr(struct sockaddr *local_ip, int type,
					    uint8_t * msgelem, int len);



extern int cw_send_echo_response(struct conn *conn, int seqnum,
				 struct radioinfo *radioinfo);
extern int cw_handle_echo_request(void *d);
extern void cw_send_image_file(struct conn *conn, FILE * infile);

extern int cw_readmsg_configuration_status_response(uint8_t * elems, int elems_len);
extern int cw_readmsg_configuration_update_request(uint8_t * elems, int elems_len);



/* Use some macros from LWAPP */

#define cw_put_byte lw_put_byte
#define cw_put_word lw_put_word
#define cw_put_dword lw_put_dword
#define cw_put_data lw_put_data
#define cw_put_bstr lw_put_bstr
#define cw_put_bstr16 lw_put_bstr16

#define cw_set_dword lw_set_dword


#define cw_get_byte lw_get_byte
#define cw_get_word lw_get_word
#define cw_get_dword lw_get_dword


/* macro to isolate bits from a dword */
#define cw_get_dword_bits(src,start,len) ((~(0xFFFFFFFF<<len)) & (src >> (32 - start - len)))

/* macros to acces transport header values */

#define cw_get_hdr_preamble(th) (th[0])
#define cw_get_hdr_fragid(th) ((ntohl((((uint32_t*)th)[1]) >> 16) & 0xffff))
#define cw_get_hdr_fragoffset(th) ((ntohl((((uint32_t*)th)[1]) >> 3) & 0x1fff))
#define cw_get_hdr_rid(th) ((ntohl((((uint32_t*)th)[0]) >> 14) & 0x1f))
#define cw_get_hdr_wbid(th) ((ntohl(((uint32_t*)th)[0]) >> 9) & 0x1f)
#define cw_get_hdr_hlen(th) ((ntohl(((uint32_t*)th)[0]) >> 19) & 0x1f)

#define cw_get_hdr_rmac(th) (th+8)
#define cw_get_hdr_rmac_len(th) (*(th+8))
#define cw_get_hdr_rmac_size(th) cw_get_hdr_rmac_len(th)
#define cw_get_hdr_rmac_data(th) (th+9)

#define cw_get_hdr_flag_r1(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_R1 ) ? 1:0)
#define cw_get_hdr_flag_r2(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_R2 ) ? 1:0)
#define cw_get_hdr_flag_r3(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_R3 ) ? 1:0)

#define cw_get_hdr_flag_k(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_K ) ? 1:0)
#define cw_get_hdr_flag_m(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_M ) ? 1:0)
#define cw_get_hdr_flag_w(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_W ) ? 1:0)
#define cw_get_hdr_flag_l(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_L ) ? 1:0)
#define cw_get_hdr_flag_f(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_F ) ? 1:0)
#define cw_get_hdr_flag_t(th) ((ntohl( *((uint32_t*)th)) & CWTH_FLAGS_T ) ? 1:0)

#define cw_get_hdr_msg_offset(th) (4*cw_get_hdr_hlen(th))
#define cw_get_hdr_msg_elems_offset(th) (cw_get_hdr_msg_offset(th)+8)

#define cw_set_hdr_preamble(th,v) ((*th)=v)




/**
 * Set the HLEN field of a CAWAP Header
 * @param th pointer to the header
 * @param hlen value to set (Header Length)
 */
static inline void cw_set_hdr_hlen(uint8_t * th, int hlen)
{
	uint32_t d = cw_get_dword(th);
	d &= (0x1f << 19) ^ 0xffffffff;
	d |= ((hlen) & 0x1f) << 19;
	cw_set_dword(th, d);
}

/**
 * Set the WBID field of a CAWAP Header
 * @param th pointer to the header
 * @param wbid value to set (Wireless ID)
 */
static inline void cw_set_hdr_wbid(uint8_t * th, int wbid)
{
	uint32_t d = cw_get_dword(th);
	d &= (0x1f << 9) ^ 0xffffffff;
	d |= ((wbid) & 0x1f) << 9;
	cw_set_dword(th, d);
}

/**
 * Set the RID field of a CAWAP Header
 * @param th pointer to the header
 * @param rid value to set (Radio ID)
 */
static inline void cw_set_hdr_rid(uint8_t * th, int rid)
{
	uint32_t d = cw_get_dword(th);
	d &= (0x1f << 9) ^ 0xffffffff;
	d |= ((rid) & 0x1f) << 14;
	cw_set_dword(th, d);
}




/**
 * Set CAPWAP header flags
 * @param th pointer to header
 * @param flags list of flags to set or unset
 * @param set 1=set flag, 0=unset flag
 */
#define cw_set_hdr_flags(th,flags,set) \
	( set ? ((*((uint32_t*)th)) |= htonl(flags)) : ((*((uint32_t*)th)) &= (0xffffffff^htonl(flags))) )

#define cw_set_hdr_flag_f(th,set)  cw_set_hdr_flag(th, CW_FLAG_HDR_F)



/* Macros for message headers */

#define cw_get_msg_id(msgptr) (cw_get_dword(msgptr))
#define cw_get_msg_type(msgptr) cw_get_msg_id(msgptr)

#define cw_get_msg_seqnum(msgptr) cw_get_byte( (msgptr) +4 )
#define cw_get_msg_elems_len(msgptr) ( cw_get_word( (msgptr) +5 )-3)
#define cw_get_msg_elems_ptr(msgptr) ((msgptr)+8)


#define cw_set_msg_id(msgptr,t) cw_put_dword(msgptr,t)
#define cw_set_msg_type(msgptr,t) cw_set_msg_id(msgptr,t)
#define cw_set_msg_seqnum(msgptr,s) cw_put_byte( (msgptr) +4,s);
#define cw_set_msg_elems_len(msgptr,n) (cw_put_word((msgptr)+5,((n)+3)))

#define cw_set_msg_flags(msgptr,f) (cw_put_byte( (msgptr)+7,f))

static inline uint8_t *cw_get_hdr_msg_elems_ptr(uint8_t * m)
{
	return cw_get_msg_elems_ptr(m + cw_get_hdr_msg_offset(m));
}

static inline uint8_t *cw_get_hdr_msg_ptr(uint8_t * rawmsg)
{
	return rawmsg + cw_get_hdr_msg_offset(rawmsg);
}

#define cw_get_hdr_msg_id(ptr)\
	cw_get_msg_id(cw_get_hdr_msg_ptr(ptr))
#define cw_get_hdr_msg_type cw_get_hdr_msg_id

static inline int cw_get_hdr_msg_total_len(uint8_t * rawmsg)
{

	int offset = cw_get_hdr_msg_offset(rawmsg);
	return offset + cw_get_msg_elems_len(rawmsg + offset) + 8;
}



static inline int cw_set_hdr_rmac(uint8_t * th,bstr_t rmac)
{
	if (!rmac){
		cw_set_hdr_flags(th,CW_FLAG_HDR_M,0);
		cw_set_hdr_hlen(th, 2);
		return 0;
	}
	int rmac_len = bstr_len(rmac);
	memcpy(cw_get_hdr_rmac(th),rmac,rmac_len+1);
	cw_set_hdr_flags(th,CW_FLAG_HDR_M,1);

	int hlen =  4+rmac_len/4;
	
	if (rmac_len %4 != 0) {
		hlen++;
	}
	cw_set_hdr_hlen(th,hlen);
	return 1;
}





/**
 * Get length of a CAPWAP message elemet 
 * @param e pointer to element (uint8_t*)
 * @return length of element
 */
#define cw_get_elem_type(e) cw_get_word(e)
#define cw_get_elem_id(e) cw_get_elem_type(e)

/**
 * Get type of a CAPWAP message element
 * @pram e pointer to element (uint8_t*)
 * @return type of element
 */
#define cw_get_elem_len(e) cw_get_word((e)+2)

/**
 * Get a pointer to the data of a CAPWAP message element 
 * @param e pointer to message element 
 * @return pointer to data
 */
#define cw_get_elem_data(e) ((e)+4)

/** 
 * Iterate through message elements of a CAPWAP message
 * @param i iterator witch points to the current element (uint8_t*)
 * @param elems pointer to message elements (uint8_t*)
 * @param len length of message element buffer
 * 
 * You can use this macro like a for loop.
 * 
 * uint8_t * i
 * cw_foreach_elem(i,elem,len){
 *   ...
 *   print_message(i);
 *   ...
 * }
 */
#define cw_foreach_elem(i,elems,len) for(i=elems; i<elems+len; i=i+4+cw_get_elem_len(i))


/**
 * Put a message element header to buffer
 * @param dst pointer to buffer (uint8_t)
 * @param type tpe of message element
 * @param len length of message element data
 * @return the number bytes put (always 4)
 */

#define cw_put_elem_hdr(dst,type,len) \
	(cw_put_dword(dst, (((uint32_t)type)<<16) | (len)))


/** 
 * Put a message element header for a message to contain a vendor specific payload
 * @param dst pointer to destination buffer
 * @param vendorid vendorid
 * @param elemid element id of vendor specific data
 * @len length of vendor specific data 
 * @return the number of bytes put (always 10)
 */
static inline int cw_put_elem_vendor_hdr(uint8_t * dst, uint32_t vendorid,
					 uint16_t elemid, uint16_t len)
{

	cw_put_elem_hdr(dst, CW_ELEM_VENDOR_SPECIFIC_PAYLOAD, len + 6);
	cw_put_dword(dst + 4, vendorid);
	cw_put_word(dst + 8, elemid);
	return 10;
}



#define cw_put_sockaddr lw_put_sockaddr

int cw_put_image_data(uint8_t *dst,FILE *infile);


/**
 * Add a message element to a buffer
 * @param dst pointer to buffer
 * @type message element type
 * @data pointer to data
 * @length of message element 
 * @return the number of bytes put
 */
static inline int cw_addelem(uint8_t * dst, uint16_t type, uint8_t * data, uint16_t len)
{
	int l = cw_put_elem_hdr(dst, type, len);
	return l + cw_put_data(dst + l, data, len);
}


static inline int cw_addelem_bstr(uint8_t * dst, uint16_t type, const bstr_t bstr)
{
	return cw_addelem(dst, type, bstr_data(bstr), bstr_len(bstr));
}


static inline int cw_put_elem_result_code(uint8_t * dst, uint32_t code)
{
	cw_put_dword(dst + 4, code);
	return 4 + cw_put_elem_hdr(dst, CW_ELEM_RESULT_CODE, 4);
}


/*
static inline int cw_addelem_radio_operational_state(uint8_t * dst, struct radioinfo *ri)
{
	cw_put_byte(dst + 4 + 0, ri->rid);
	cw_put_byte(dst + 4 + 1, ri->state);
	cw_put_byte(dst + 4 + 2, ri->cause);
	return 3 + cw_put_elem_hdr(dst, CW_ELEM_RADIO_OPERATIONAL_STATE, 3);
}
*/


/*
#define cw_put_elem_vendor_hdr(dst,vendorid,elemid,len)\
	(cw_put_elem_hdr(dst,CW_ELEM_VENDOR_SPECIFIC_PAYLOAD,  \
	cw_put_dword(dst+4,vendorid) + cw_put_word(dst+8,elemid) +len ))





#define cw_addelem(dst,type,data,len)\
	(cw_put_elem_hdr(dst,type,len)+cw_put_data(dst+4,data,len))
*/

/*
#define cw_addelem_vendor_specific_payload(dst,vendorid,elemid,data,len)\
	(cw_put_elem_vendor_hdr(dst,vendorid,elemid,len) + \
	 cw_put_data(dst+10,data,len))
*/

/*
extern int cw_addelem_vendor_specific_payload(uint8_t * dst, uint32_t vendorid,
					      uint16_t elemid, uint8_t * data,
					      uint16_t len);


extern void cw_prepare_configuration_status_request(struct conn *conn,
						    struct radioinfo *radioinfo,
						    struct wtpinfo *wtpinfo);
extern void cw_prepare_change_state_event_request(struct conn *conn,
						  struct radioinfo *radioinfo,
						  struct wtpinfo *wtpinfo);

extern int cw_send_configuration_update_response(struct conn *conn, int seqnum,
						 struct radioinfo *radioinfo);
*/

#define cw_addelem_ac_name(dst,name) \
	cw_addelem(dst,CW_ELEM_AC_NAME,name,strlen((char*)(name)))

#define cw_addelem_session_id(dst,sessid)\
	cw_addelem_bstr(dst,CW_ELEM_SESSION_ID,sessid)



/* cwmsg methods */
/*
static inline int cw_addelem_result_code(uint8_t * dst, uint32_t code)
{
	cw_put_dword(dst + 4, code);
	return 4 + cw_put_elem_hdr(dst, CW_ELEM_RESULT_CODE, 4);
}
*/


/* Message to text stuff */

/* Constants to string conversion lists */
extern struct cw_strlist_elem capwap_strings_msg[];
extern struct cw_strlist_elem capwap_strings_state[];
extern struct cw_strlist_elem capwap_strings_vendor[];
extern struct cw_strlist_elem capwap_strings_elem[];
extern struct cw_strlist_elem capwap_strings_result[];
extern struct cw_strlist_elem mbag_item_strings[];




#define cw_strmsg(id) cw_strlist_get_str(capwap_strings_msg,id)
#define cw_strelem(id) cw_strlist_get_str(capwap_strings_elem,id)
#define cw_strstate(id) cw_strlist_get_str(capwap_strings_state,id)
#define cw_strvendor(id) cw_strlist_get_str(capwap_strings_vendor,id)
#define cw_strresult(id) cw_strlist_get_str(capwap_strings_result,(id))

#define cw_stritem(id) cw_strlist_get_str(mbag_item_strings,(id))

/*
#define cw_strrc(rc) \
	((rc)<0 ? ((rc)!=EAGAIN ? strerror(errno):"Timed out"): cw_strresult(rc))
#define cw_strerror(rc) cw_strrc(rc)
*/

static inline const char * cw_strerror(int rc) {
	if (rc<0){
		if (errno==EAGAIN)
			return "Timed out";
		return strerror(errno);
	}
	return cw_strresult(rc);
}
#define cw_strrc cw_strerror

static inline int cw_rcok(int rc)
{
	if (rc==0 || rc==2){
		return 1;
	}
	return 0;
}



static inline const char *cw_strelemp_(cw_strheap_t h, int msg_id)
{
	const char *rc = cw_strheap_get(h, msg_id);
	if (rc)
		return rc;
	return cw_strheap_get(h, CW_STR_STOP);
}

#define cw_strelemp(p,id) cw_strelemp_((p)->strelem,id)





//int cw_process_msg(struct conn *conn, uint8_t * rawmsg, int len);


extern int cw_in_generic(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from);
extern int cw_in_vendor_specific_payload(struct conn *conn, struct cw_action_in *a,
					 uint8_t * data, int len,struct sockaddr *from);
extern int cw_in_wtp_name(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			  int len,struct sockaddr *from);
extern int cw_in_wtp_board_data(struct conn *conn, struct cw_action_in *a, uint8_t * data,
				int len,struct sockaddr *from);
extern int cw_in_wtp_descriptor(struct conn *conn, struct cw_action_in *a, uint8_t * data,
				int len,struct sockaddr *from);
extern int cw_in_capwap_control_ipv4_address(struct conn *conn, struct cw_action_in *a,
					     uint8_t * data, int len, struct sockaddr *from);

//extern int cw_out_generic(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len);
extern int cw_out_generic(struct conn *conn, struct cw_action_out *a, uint8_t * dst);	//, struct mbag_item *item);

//extern int cw_out_ac_descriptor(struct conn *conn, uint32_t elem_id, uint8_t * dst,
				//struct mbag_item *item);
extern int cw_out_ac_descriptor(struct conn *conn, struct cw_action_out *a, uint8_t * dst);	//,struct mbag_item * item) 
extern int cw_out_wtp_descriptor(struct conn *conn, struct cw_action_out *a,
				 uint8_t * dst);


//extern int cw_out_capwap_control_ip_addrs(struct conn *conn, uint32_t elem_id,
//                                        uint8_t * dst, struct mbag_item *item);

extern int cw_out_capwap_control_ip_addr_list(struct conn *conn, struct cw_action_out *a,
					      uint8_t * dst);

extern int cw_put_msg(struct conn *conn, uint8_t * rawout);
int cw_out_image_data(struct conn *conn, struct cw_action_out *a, uint8_t * dst);	// ,struct mbag_item * item) 


struct cw_ac_status {
	int stations;
	int limit;
	int active_wtps;
	int max_wtps;
	int security;
	int rmac_field;
	int dtls_policy;
};

/**
 * Put an cw_ac_stauts structure to a buffer
 * @param dst destination buffer
 * @param s #cw_ac_status to put
 * @return number of bytes put
 * This function is only useful (used) in conjunction with 
 * putting AC Descriptor message elements.
 */
static inline int cw_put_ac_status(uint8_t * dst, struct cw_ac_status *s)
{
	uint8_t *d = dst;

	d += cw_put_dword(d, (s->stations << 16) | (s->limit));
	d += cw_put_dword(d, (s->active_wtps << 16) | (s->max_wtps));
	d += cw_put_dword(d,
			  (s->security << 24) | (s->rmac_field << 16) | (s->dtls_policy));
	return d - dst;
}


static inline int cw_put_version(uint8_t * dst, uint16_t subelem_id, uint8_t * v)
{
	uint8_t *d = dst;
	d += cw_put_dword(d, vendorstr_get_vendor_id(v));
	d += cw_put_dword(d, (subelem_id << 16) | vendorstr_len(v));
	d += cw_put_data(d, vendorstr_data(v), vendorstr_len(v));
	return d - dst;
}



int cw_register_actions_capwap_ac(struct cw_actiondef *def);
int cw_register_actions_cipwap_ac(struct cw_actiondef *def);
int cw_register_actions_capwap_wtp(struct cw_actiondef *def);


int cw_in_set_state_none(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len);
struct mbag_item *cw_out_get_outgoing(struct conn *conn, struct cw_action_out *a);
struct mbag_item *cw_out_get_local(struct conn *conn, struct cw_action_out *a);
extern int cw_in_check_join_resp(struct conn *conn, struct cw_action_in *a,
				 uint8_t * data, int len,struct sockaddr *from);

extern int cw_in_check_disc_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
				int len,struct sockaddr *from);
int cw_in_check_disc_resp(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			  int len,struct sockaddr *from);

int cw_check_missing_mand(cw_action_in_t ** out, struct conn *conn, cw_action_in_t * a);
int cw_in_check_join_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from);
extern int cw_in_check_img_data_req_wtp(struct conn *conn, struct cw_action_in *a,
					uint8_t * data, int len,struct sockaddr *from);
extern int cw_in_check_img_data_req_ac(struct conn *conn, struct cw_action_in *a,
				       uint8_t * data, int len,struct sockaddr *from);
int cw_in_check_img_data_resp(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			      int len,struct sockaddr *from);

int cw_out_wtp_board_data(struct conn *conn, struct cw_action_out *a, uint8_t * dst);

void cw_init_request(struct conn *conn, int msg_id);
struct mbag_item *cw_out_get_session_id(struct conn *conn, struct cw_action_out *a);
struct mbag_item *cw_out_get_config(struct conn *conn, struct cw_action_out *a);


extern int cw_in_radio_administrative_state(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from);

int cw_out_ac_name_with_priority(struct conn *conn, struct cw_action_out *a, uint8_t * dst);

int cw_send_request(struct conn *conn, int msg_id);
int cw_is_utf8(unsigned char *str, size_t len);

int cw_in_ac_name_with_priority(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from);

int cw_out_radio_administrative_states(struct conn *conn, struct cw_action_out *a, uint8_t * dst);

int cw_in_radio_administrative_state(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from);
int cw_in_ac_descriptor(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len,struct sockaddr *from);

int cw_in_radio_generic(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from);

int cw_out_capwap_local_ip_address(struct conn *conn, struct cw_action_out *action, uint8_t * dst);

int cw_in_check_chng_state_evnt_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from);


int cw_out_radio_operational_states(struct conn *conn, struct cw_action_out *a, uint8_t * dst);


int cw_in_check_cfg_update_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from);

int cw_radio_set_admin_state(mbag_t radios,int rid, int state,int cause);

int cw_in_radio_administrative_state_wtp(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from);


#endif
