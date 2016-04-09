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

//#include "cw.h"

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



/**@}*/



/**
 * CAPWAP header flags.
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
#define CW_WBID_RESERVED1	0
#define CW_WBID_IEEE80211	1
#define CW_WBID_RESERVED2	2
#define CW_WBID_EPCGLOBAL	3


#define CAPWAP_PACKET_PREAMBLE (CAPWAP_VERSION<<4)
#define CAPWAP_DTLS_PACKET_PREAMBLE (CAPWAP_VERSION<<4|1)

/*
 * control header stuff
 */

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
/** Config. Update Request = 7 */
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
#define CW_ELEM_AC_NAME_WITH_INDEX			CW_ELEM_AC_NAME_WITH_PRIORITY	/* Draft 7 naming */
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
#define CW_ELEM_ECN_SUPPORT				53
#define CW_ELEM_IDLE_TIMEOUT				23
#define CW_ELEM_IMAGE_DATA				24
#define CW_ELEM_IMAGE_IDENTIFIER			25
#define CW_ELEM_IMAGE_INFORMATION			26
#define CW_ELEM_INITIATE_DOWNLOAD			27
#define CW_ELEM_LOCATION_DATA				28
#define CW_ELEM_MAXIMUM_MESSAGE_LENGTH			29
#define CW_ELEM_MTU_DISCOVERY_PADDING			52
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



/**
 * @defgrpup BOARD_SUBELEMS Board Data Sub-Elements
 * @{
 */
#define CW_BOARDDATA_MODELNO		0
#define CW_BOARDDATA_SERIALNO		1
#define CW_BOARDDATA_BOARDID		2
#define CW_BOARDDATA_REVISION		3
#define CW_BOARDDATA_MACADDRESS		4
/**
 * @}
 */



#define CW_FLAG_RMAC_RESERVED		0
#define CW_FLAG_RMAC_SUPPORTED		1
#define CW_FLAG_RMAC_NOT_SUPPORTED	2


#define CW_SUBELEM_WTP_HARDWARE_VERSION			0
#define CW_SUBELEM_WTP_SOFTWARE_VERSION			1
#define CW_SUBELEM_WTP_BOOTLOADER_VERSION		2
#define CW_SUBELEM_WTP_OTHERSOFTWARE_VERSION		3

#define CW_SUBELEM_AC_HARDWARE_VERSION			4
#define CW_SUBELEM_AC_SOFTWARE_VERSION			5


/* Frame tunnnel mode bits */
#define WTP_FRAME_TUNNEL_MODE_R	1	/* Reserved */
#define WTP_FRAME_TUNNEL_MODE_L	2	/* Local bridging */
#define WTP_FRAME_TUNNEL_MODE_E	4	/* 802.3 mode */
#define WTP_FRAME_TUNNEL_MODE_N	8	/* native mode */

#include "radioinfo.h"


#define CW_DISCOVERY_TYPE_UNKNOWN	0
#define CW_DISCOVERY_TYPE_STATIC	1
#define CW_DISCOVERY_TYPE_DHCP		2
#define CW_DISCOVERY_TYPE_DNS		3
#define CW_DISCOVERY_TYPE_AC_REFERRAL	4




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

#define CAPWAP_IDLE_TIMEOUT		300


#define CAPWAP_STATISTICS_TIMER		120


#define CAPWAP_TIMERS			((CAPWAP_DISCOVERY_INTERVAL << 8) | CAPWAP_ECHO_INTERVAL)


//#define CAPWAP_CIPHER                 "PSK-AES128-CBC-SHA:"
//#define CAPWAP_CIPHER "AES128-SHA"

#ifdef WITH_GNUTLS
#define CAPWAP_CIPHER	"NORMAL"
#else
#define CAPWAP_CIPHER	"ALL"
#endif



/**
 * @defgroup  ACSECURITY Security Flags used in AC Descriptor 
 * @{
 */
/** X.509 Support */
#define CW_FLAG_AC_SECURITY_X			2
/* PSK Support */
#define CW_FLAG_AC_SECURITY_S			4
/**
 * @}
 */


/**
 * @defgroup ACDTLSPOLICY AC DTLS Policy Flags  
 * @{
 */
/** Reserved */
#define CW_FLAG_DTLS_POLICY_R		1
/** Clear Text Data Channel Support  */
#define CW_FLAG_DTLS_POLICY_C		2
/** DTLS Data Channel Support */
#define CW_FLAG_DTLS_POLICY_D		4
/**
 * @}
 */



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



//extern void cwmsg_addelem_wtp_radio_infos(struct cwmsg * cwmsg,struct wtpinfo * wtpinfo);
extern void cwmsg_addelem_wtp_radio_infos(struct cwmsg *msg,
					  struct radioinfo *radioinfos);

//extern void cwmsg_addelem_result_code(struct cwmsg *msg, int rc);
extern void cwmsg_addelem_vendor_specific_payload(struct cwmsg *msg, int vendor_id,
						  int type, uint8_t * payload, int len);



extern int hdr_print(char *str, uint8_t * packet, int len);

extern int cw_readelem_ecn_support(uint8_t * ecn_support, int type, uint8_t * msgelem,
				   int len);
//extern int cw_readelem_maximum_message_length(uint16_t * dst, int type, uint8_t * msgelem,
//					      int len);
//extern int cw_readelem_ac_name(uint8_t ** dst, int type, uint8_t * msgelem, int len);
//extern int cw_readelem_wtp_reboot_statistics(struct wtp_reboot_statistics *s, int type,
//					     uint8_t * msgelem, int len);
//extern int cw_readelem_cw_local_ip_addr(struct sockaddr *local_ip, int type,
//					uint8_t * msgelem, int len);
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





enum cw_reboot_failure_types {
	CW_REBOOT_TYPE_NOT_SUPPORTED=0,
	CW_REBOOT_TYPE_AC_INITIATED=1,
	CW_REBOOT_TYPE_LINK_FAILURE=2,
	CW_REBOOT_TYPE_SOFTWARE_FAILURE=3,
	CW_REBOOT_TYPE_HARDWARE_FAILURE=4,
	CW_REBOOT_TYPE_OTHER_FAILURE=5,
	CW_REBOOT_TYPE_FAILURE_ONKNOWN

};


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


#define CW_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT			21

/*      22 Data Transfer Error (No Information to Transfer)

*/



#define CW_RADIO_ADMIN_STATE_ENABLED	1
#define CW_RADIO_ADMIN_STATE_DISABLED	2

#define CW_RADIO_OPER_STATE_ENABLED	1
#define CW_RADIO_OPER_STATE_DISABLED	2

#define CW_RADIO_OPER_STATE_ENABLED_7	2
#define CW_RADIO_OPER_STATE_DISABLED_7	1


extern void cw_read_image_data_request(struct cwimage_data *, uint8_t * msg, int len);

//extern int cw_readelem_ac_descriptor(struct ac_info *acinfo, int type, uint8_t * msgelem,
//				     int len);
//extern int cw_readelem_capwap_local_ip_addr(struct sockaddr *local_ip, int type,
//					    uint8_t * msgelem, int len);



extern int cw_send_echo_response(struct conn *conn, int seqnum,
				 struct radioinfo *radioinfo);
extern int cw_handle_echo_request(void *d);
extern void cw_send_image_file(struct conn *conn, FILE * infile);

extern int cw_readmsg_configuration_status_response(uint8_t * elems, int elems_len);
extern int cw_readmsg_configuration_update_request(uint8_t * elems, int elems_len);



/* Message to text stuff */

/* Constants to string conversion lists */
extern struct cw_strlist_elem capwap_strings_msg[];
extern struct cw_strlist_elem capwap_strings_state[];
extern struct cw_strlist_elem capwap_strings_vendor[];
extern struct cw_strlist_elem capwap_strings_elem[];
extern struct cw_strlist_elem capwap_strings_result[];
extern struct cw_strlist_elem capwap_strings_board[];
extern struct cw_strlist_elem mbag_item_strings[];






#define cw_strmsg(id) cw_strlist_get_str(capwap_strings_msg,id)
#define cw_strelem(id) cw_strlist_get_str(capwap_strings_elem,id)
#define cw_strstate(id) cw_strlist_get_str(capwap_strings_state,id)
#define cw_strvendor(id) cw_strlist_get_str(capwap_strings_vendor,id)
#define cw_strresult(id) cw_strlist_get_str(capwap_strings_result,id)
#define cw_strboardelem(id) cw_strlist_get_str(capwap_strings_board,id)

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


/**
 *@defgroup CW CW 
 *@{ 
 */

static inline const char *cw_strelemp_(cw_strheap_t h, int msg_id)
{
	const char *rc = cw_strheap_get(h, msg_id);
	if (rc)
		return rc;
	return cw_strheap_get(h, CW_STR_STOP);
}

/**@}*/

#define cw_strelemp(p,id) cw_strelemp_((p)->strelem,id)





//int cw_process_msg(struct conn *conn, uint8_t * rawmsg, int len);


extern int cw_in_generic(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from);
extern int cw_in_generic2(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from);

extern int cw_in_vendor_specific_payload(struct conn *conn, struct cw_action_in *a,
					 uint8_t * data, int len,struct sockaddr *from);
extern int cw_in_wtp_name(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			  int len,struct sockaddr *from);
extern int cw_in_wtp_board_data(struct conn *conn, struct cw_action_in *a, uint8_t * data,
				int len,struct sockaddr *from);
extern int cw_in_wtp_descriptor(struct conn *conn, struct cw_action_in *a, uint8_t * data,
				int len,struct sockaddr *from);
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
 *@defgroup CAPWAP CAPWAP
 *@{
 */

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

/**
 *@}
 */



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
//int cw_is_utf8(unsigned char *str, size_t len);

int cw_in_ac_name_with_priority(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from);

int cw_out_radio_administrative_states(struct conn *conn, struct cw_action_out *a, uint8_t * dst);

int cw_in_radio_administrative_state(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from);
int cw_in_ac_descriptor(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len,struct sockaddr *from);

int cw_in_radio_generic(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from);

//int cw_out_capwap_local_ip_address(struct conn *conn, struct cw_action_out *action, uint8_t * dst);

int cw_in_check_chng_state_evnt_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from);


int cw_out_radio_operational_states(struct conn *conn, struct cw_action_out *a, uint8_t * dst);


int cw_in_check_cfg_update_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from);

int cw_radio_set_admin_state(mbag_t radios,int rid, int state,int cause);

int cw_in_radio_administrative_state_wtp(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from);





#endif
