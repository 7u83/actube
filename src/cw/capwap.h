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
 * @brief CAPWAP definitions as in RFC 5415
 */


#ifndef __CAPWAP_H
#define __CAPWAP_H

#include <stdint.h>


#include <errno.h>
#include <arpa/inet.h>

#include "conn.h"

#include "strlist.h"


/* capwap version and iana number */

/**
 * @defgroup CAPWAP CAPWAP 
 * @{
 */


/** 
 * @defgroup CAPWAP_DEFS   Definitions from RFC5415
 * @{
 */

/**
 *@defgroup CAPWAPConstants General Constants 
 *@brief General constants as defined in RFC5415
 *@{
 */

/** CAPWAP Version */
#define CAPWAP_VERSION (0)

#define CAPWAP_IANA_ENTERPRISE_NUMBER 0
 
/** CAPWAP Control Port. This port is defined by RFC5415
 * for the AC to listen on for CAPWAP control messages. */
#define CAPWAP_CONTROL_PORT 5246

/** CAPWAP Control Port as String.
 * @see CAPWAP_CONTROL_PORT */
#define CAPWAP_CONTROL_PORT_STR "5246"

/** CAPWAP Data Port */
#define CAPWAP_DATA_PPORT 5247

/** CAWPAP DataPort as string constant.
 * @see CAPWAP_DATA_PORT */
#define CAPWAP_DATA_PORT_STR "5247"

/**@}*/



/**
 *@defgroup CAPWAPHeaderFlags Header Flags
 *@brief The CAPWAP control header flags
 * 
 *@{
 */
#define CAPWAP_FLAG_HDR_R1 0x01		/**< bit 0 reserved 1 */
#define CAPWAP_FLAG_HDR_R2 0x02		/**< bit 1 reserved 2 */
#define CAPWAP_FLAG_HDR_R3 0x04		/**< bit 2 reserved 3 */
#define CAPWAP_FLAG_HDR_K  0x08		/**< bit 3 Keep alive flag */
#define CAPWAP_FLAG_HDR_M  0x10		/**< bit 4 MAC Adress field present */
#define CAPWAP_FLAG_HDR_W  0x20		/**< bit 5 wireless info present */
#define CAPWAP_FLAG_HDR_L  0x40		/**< bit 6 last fragment */
#define CAPWAP_FLAG_HDR_F  0x80		/**< bit 7 fragment */
#define CAPWAP_FLAG_HDR_T  0x100	/**< bit 8 type of payload frame */
/**@}*/




/**
 * @defgroup CAPWAPWBIDS Wireless binding IDs
 * @{
 */
#define CAPWAP_WBID_RESERVED1	0
#define CAPWAP_WBID_IEEE80211	1
#define CAPWAP_WBID_RESERVED2	2
#define CAPWAP_WBID_EPCGLOBAL	3
/**@}*/

#define CAPWAP_PACKET_PREAMBLE (CAPWAP_VERSION<<4)
#define CAPWAP_DTLS_PACKET_PREAMBLE (CAPWAP_VERSION<<4|1)


 

/**
 * @defgroup CAPWAPMessages  Message types as defined in RFC 5416
 * @{
 */
/**
 * The Discovery Request message is sent by the WTP to 
 * find an AC on the network and inform the AC about some 
 * basic capabillities of the WTP. */
#define	CAPWAP_MSG_DISCOVERY_REQUEST			1

/**
 * A Discovery Response message is send by the AP to 
 * the WTP after receiving a Discovery Request message. */
#define CAPWAP_MSG_DISCOVERY_RESPONSE			2

/**
 * The CAPWAP Join Request message type. A Join Request is 
 * snet by the WTP to join an AC */
#define	CAPWAP_MSG_JOIN_REQUEST				3

/**
 * Join Response, sent by the AC to the WTP in response to 
 * Join Request message */
#define CAPWAP_MSG_JOIN_RESPONSE			4

/**
 * The Config Status Request message is sent by the WTP
 * to report ist status to the AC*/
#define CAPWAP_MSG_CONFIGURATION_STATUS_REQUEST		5

#define CAPWAP_MSG_CONFIGURATION_STATUS_RESPONSE	6

#define CAPWAP_MSG_CONFIGURATION_UPDATE_REQUEST		7

#define CAPWAP_MSG_CONFIGURATION_UPDATE_RESPONSE	8

#define CAPWAP_MSG_WTP_EVENT_REQUEST			9
#define CAPWAP_MSG_WTP_EVENT_RESPONSE			10

#define CAPWAP_MSG_CHANGE_STATE_EVENT_REQUEST		11
#define CAPWAP_MSG_CHANGE_STATE_EVENT_RESPONSE		12

#define CAPWAP_MSG_ECHO_REQUEST				13
#define CAPWAP_MSG_ECHO_RESPONSE			14

#define CAPWAP_MSG_IMAGE_DATA_REQUEST			15
#define CAPWAP_MSG_IMAGE_DATA_RESPONSE			16

/**
 * A Reset Request message ist sent by the AC to the WTP 
 * to initiate a WTP reset.*/
#define CAPWAP_MSG_RESET_REQUEST			17
#define CAPWAP_MSG_RESET_RESPONSE			18

#define CAPWAP_MSG_PRIMARY_DISCOVERY_REQUEST		19
#define CAPWAP_MSG_PRIMARY_DISCOVERY_RESPONSE		20

#define CW_MSG_DATA_TRANSFER_REQUEST			21
#define CW_MSG_DATA_TRANSFER_RESPONSE			22

#define CW_MSG_CLEAR_CONFIGURATION_REQUEST		23
#define CW_MSG_CLEAR_CONFIGURATION_RESPONSE		24

#define CW_STATION_CONFIGURATION_REQUEST		25
#define CW_STATION_CONFIGURATION_RESPONSE		26

#define CW_MSG_MAXMSG					26

/**@}*/


/**
 * @defgroup CAPWAPMsgElems Message Elements 
 * @brief CAPWAP message elements as defined in  RFC 5415
 * @{
 */   


#define CAPWAP_ELEM_AC_DESCRIPTOR			1	/**< AC Descriptor */

#define CAPWAP_ELEM_AC_IPV4_LIST			2	/**< AC IP V4 List */
#define CAPWAP_ELEM_AC_IPV6_LIST			3	/**< AC IP V6 List */
#define CAPWAP_ELEM_AC_NAME				4	/**< AC Name */
#define CAPWAP_ELEM_AC_NAME_WITH_PRIORITY		5	/**< AC Name with Priority */

/** 
 * In CAPWAP RFC draft 7 the message element 5 was named
 * AC Name With Index. So we define it here with this name
 * to be compatible. 
 * @see CW_ELEM_AC_NAME_WITH_PRIORITY */
#define CW_ELEM_AC_NAME_WITH_INDEX			CAPWAP_ELEM_AC_NAME_WITH_PRIORITY

#define CAPWAP_ELEM_AC_TIMESTAMP			6
/**@{
 * one for all
 */
 /* yes one for alllll */
#define CW_ELEM_ADD_MAC_ACL_ENTRY			7
#define CW_ELEM_ADD_STATION				8
#define CW_ELEM_RESERVED_9				9
#define CAPWAP_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS		10
#define CAPWAP_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS		11
#define CAPWAP_ELEM_CAPWAP_TIMERS			12

/**@}*/
#define CAPWAP_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS		30
#define CAPWAP_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS		50

#define CW_ELEM_CAPWAP_TRANSPORT_PROTOCOL		51	/* not in draft 7 */
#define CW_ELEM_DATA_TRANSFER_DATA			13
#define CW_ELEM_DATA_TRANSFER_MODE			14
#define CW_ELEM_DECRYPTION_ERROR_REPORT			15
#define CAPWAP_ELEM_DECRYPTION_ERROR_REPORT_PERIOD	16
#define CW_ELEM_DELETE_MAC_ACL_ENTRY			17
#define CW_ELEM_DELETE_STATION				18
#define CW_ELEM_RESERVED_19				19
/**
 * Indicates the AP's Discovery Type
 * @see CAPWAPDIscoveryTypes */
#define CAPWAP_ELEM_DISCOVERY_TYPE			20
#define CW_ELEM_DUPLICATE_IPV4_ADDRESS			21
#define CW_ELEM_DUPLICATE_IPV6_ADRESS			22
#define CAPWAP_ELEM_ECN_SUPPORT				53
#define CAPWAP_ELEM_IDLE_TIMEOUT			23
#define CW_ELEM_IMAGE_DATA				24
#define CW_ELEM_IMAGE_IDENTIFIER			25
#define CW_ELEM_IMAGE_INFORMATION			26
#define CW_ELEM_INITIATE_DOWNLOAD			27
#define CAPWAP_ELEM_LOCATION_DATA			28
#define CAPWAP_ELEM_MAXIMUM_MESSAGE_LENGTH		29
#define CAPWAP_ELEM_MTU_DISCOVERY_PADDING		52
#define CAPWAP_ELEM_RADIO_ADMINISTRATIVE_STATE		31
#define CAPWAP_ELEM_RADIO_OPERATIONAL_STATE			32
#define CAPWAP_ELEM_RESULT_CODE				33
#define CW_ELEM_RETURNED_MESSAGE_ELEMENT		34
#define CAPWAP_ELEM_SESSION_ID				35
#define CAPWAP_ELEM_STATISTICS_TIMER			36
/**
 * The Vendor Specific Payload allows tronasport of 
 * vebdor defined data.*/
#define CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD		37

#define CAPWAP_ELEM_WTP_BOARD_DATA			38
/**
 * The WTP Descriptor message element conteins information
 * about the WTP */
#define CAPWAP_ELEM_WTP_DESCRIPTOR			39
#define CAPWAP_ELEM_WTP_FALLBACK			40
#define CAPWAP_ELEM_WTP_FRAME_TUNNEL_MODE		41
#define CAPWAP_ELEM_RESERVED_42				42
#define CAPWAP_ELEM_RESERVED_43				43
#define CAPWAP_ELEM_WTP_MAC_TYPE			44
#define CAPWAP_ELEM_WTP_NAME				45
#define CAPWAP_ELEM_RESERVED_46				46
#define CAPWAP_ELEM_WTP_RADIO_STATISTICS		47
#define CAPWAP_ELEM_WTP_REBOOT_STATISTICS		48
#define CAPWAP_ELEM_WTP_STATIC_IP_ADDRESS_INFORMATION	49
/*#define CAWAP_ELEM_WTP_STATIC_IP_ADDR_INFO		49*/


/* Cisco's CAPWAP definitions (CAPWAP draft 7) */
#define	CAPWAP_ELEM_WTP_IPV4_IP_ADDRESS			42
#define	CAPWAP_ELEM_WTP_IPV6_IP_ADDRESS			43

/**@}*/

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

/**
 * @defgroup DESCRIPTORSUBELEM Descriptor sub-elements
 * @{
 */
#define CW_SUBELEM_WTP_HARDWARE_VERSION			0
#define CW_SUBELEM_WTP_SOFTWARE_VERSION			1
#define CW_SUBELEM_WTP_BOOTLOADER_VERSION		2
#define CW_SUBELEM_WTP_OTHERSOFTWARE_VERSION		3

#define CAPWAP_SUBELEM_AC_HARDWARE_VERSION		4
#define CAPWAP_SUBELEM_AC_SOFTWARE_VERSION		5
/**
 * @}
 */


/**
 * @defgroup FRAMETUNNELBITS Frame-tunnel-mode Bits
 * @{
 */
 
#define CW_WTP_FRAME_TUNNEL_MODE_R	1	/**< Reserved */
#define CW_WTP_FRAME_TUNNEL_MODE_L	2	/**< Local bridging */
#define CW_WTP_FRAME_TUNNEL_MODE_E	4	/**< 802.3 mode */
#define CW_WTP_FRAME_TUNNEL_MODE_N	8	/**< native mode */
/**
 * @}
 */


/**
 * @defgroup CAPWAPDiscoveryTypes Discovery Types
 * @brief CAPWAP Discovery Types defined in RFC5415
 * @{
 */
/** Discovery Type is unknown */
#define CAPWAP_DISCOVERY_TYPE_UNKNOWN		0	
/** AC was discoverd by a static IP address stored in AP's config */
#define CAPWAP_DISCOVERY_TYPE_STATIC		1
/** AC was discovered by DHCP option 43 returned from a DHCP server */
#define CAPWAP_DISCOVERY_TYPE_DHCP		2
/** AC was discovered by DNS lookup */
#define CAPWAP_DISCOVERY_TYPE_DNS		3
/** AC was discovered by referral */
#define CAPWAP_DISCOVERY_TYPE_AC_REFERRAL	4
/**@}*/


/* WTP mac types */
#define CAPWAP_WTP_MAC_TYPE_LOCAL		0
#define CAPWAP_WTP_MAC_TYPE_SPLIT		1
#define CAPWAP_WTP_MAC_TYPE_BOTH		2


/*
//#define CWMSG_MAX_SIZE 65536
//#define CWMSG_MAX_SIZE 2048
*/

#define CAPWAP_SESSION_ID_LEN		16
#define CAPWAP_MAX_AC_NAME_LEN		512
#define CAPWAP_MAX_WTP_NAME_LEN		512
#define CAPWAP_MAX_LOCATION_DATA_LEN	1024


/* capwap timer default values */
#define CAPWAP_DISCOVERY_INTERVAL	5
#define CAPWAP_MAX_DISCOVERY_INTERVAL	20
#define CAPWAP_MIN_DISCOVERY_INTERVAL	2

#define CAPWAP_RETRANSMIT_INTERVAL	3
#define CAPWAP_MAX_DISCOVERIES		10
#define CAPWAP_MAX_RETRANSMIT		5
#define CAPWAP_SILENT_INTERVAL		30
#define CAPWAP_ECHO_INTERVAL 		30
#define CAPWAP_WAIT_DTLS		60
#define	CAPWAP_WAIT_JOIN		60

#define CAPWAP_TIMER_CHANGE_STATE_PENDING_TIMER		25
#define CAPWAP_TIMER_DATA_CHECK_TIMER			30	

#define CAPWAP_IDLE_TIMEOUT		300


#define CAPWAP_STATISTICS_TIMER		120


/*#define CW_TIMERS			((CW_DISCOVERY_INTERVAL << 8) | CAPWAP_ECHO_INTERVAL)*/

/*
//#define CAPWAP_CIPHER "PSK-AES128-CBC-SHA:"
//#define CAPWAP_CIPHER "AES128-SHA"
*/


#ifdef USE_OPENSSL
	#define CAPWAP_CIPHER	"ALL"
#else
	#define CAPWAP_CIPHER	"+RSA:+AES-128-CBC:+SHA1:"
#endif


/**
 * @defgroup  ACSECURITY Security Flags used in AC Descriptor 
 * @{
 */
/** X.509 Support */
#define CAPWAP_FLAG_AC_SECURITY_X			2
/* PSK Support */
#define CAPWAP_FLAG_AC_SECURITY_S			4
/**
 * @}
 */


/**
 * @defgroup ACDTLSPOLICY CAPWAP DTLS Policy Flags
 * The DTLS policy flags are describing the data transfer
 * policy by AC. 
 * @{
 */

/** Reserved */
#define CAPWAP_FLAG_DTLS_POLICY_R		1

/** 
 * Clear Text Data Channel Support. 
 * Means unencrypted data is supprted.*/
#define CAPWAP_FLAG_DTLS_POLICY_C		2

/** 
 * DTLS Data Channel Support. 
 * Encrypted data is supported. */
#define CAPWAP_FLAG_DTLS_POLICY_D		4
/**
 * @}
 */






enum cw_reboot_failure_types {
	CW_REBOOT_TYPE_NOT_SUPPORTED=0,
	CW_REBOOT_TYPE_AC_INITIATED=1,
	CW_REBOOT_TYPE_LINK_FAILURE=2,
	CW_REBOOT_TYPE_SOFTWARE_FAILURE=3,
	CW_REBOOT_TYPE_HARDWARE_FAILURE=4,
	CW_REBOOT_TYPE_OTHER_FAILURE=5,
	CW_REBOOT_TYPE_FAILURE_ONKNOWN

};

/**
 * @defgroup CAPWAPResultCodes CAPWAP Result Codes
 * @{
 */
 
/** The Result Success code is sent, whenever 
 * all went fine ;-) */ 
#define CAPWAP_RESULT_SUCCESS					0
/** 
 * The Missing AC List Result Code is sent by the 
 * WTP to the AC when the AC List is missing */
#define CW_RESULT_MISSING_AC_LIST				1
#define CAPWAP_RESULT_SUCCESS_NAT				2
#define CW_RESULT_JOIN_FAILURE					3
#define CW_RESULT_JOIN_RESOURCE_DEPLETION			4
#define CW_RESULT_JOIN_UNKNOWN_SOURCE				5
#define CW_RESULT_JOIN_INCORRECT_DATA				6
#define CW_RESULT_JOIN_FAILURE_SESSION_ALREADY_IN_USE		7
#define CW_RESULT_JOIN_FAILURE_WTP_HARDWARE_NOT_SUPPORTED	8
#define CW_RESULT_JOIN_FAILURE_BINDING_NOT_SUPPORTED		9
#define CW_RESULT_RESET_UNABLE_TO_RESET				10

#define CW_RESULT_RESET_FAILURE_FIRMWARE_WRITE_ERROR		11

/** 
 * Configuration Failure (Unable to Apply Requested Configuration
 * - Service Provided Anyhow)
 */ 
#define CW_RESULT_CONFIGURATION_FAILURE				12



/**

      13 Configuration Failure (Unable to Apply Requested Configuration
         - Service Not Provided)
*/
#define CAPWAP_RESULT_CONFIGURATION_FAILURE_SERVICE_NOT PROVIDED	13
/** 
 * Image Data Error (Invalid Checksum)
 */
#define CW_RESULT_IMAGE_DATA_INVALID_CHECKSUM			14 

/**
 * Image Data Error (Invalid Data Length)
 */
#define CW_RESULT_IMAGE_DATA_IVALID_LENGTH			15

/**
 * Image Data Error (Other Error)
 */
#define CAPWAP_RESULT_IMAGE_DATA_ERROR				16

/*
      17 Image Data Error (Image Already Present)
*/

#define CAPWAP_RESULT_MSG_INVALID_IN_CURRENT_STATE		18


#define CAPWAP_RESULT_MSG_UNRECOGNIZED				19

#define CW_RESULT_MISSING_MAND_ELEM				20


#define CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT		21

/*      22 Data Transfer Error (No Information to Transfer)

*/



#define CW_RADIO_ADMIN_STATE_ENABLED	1
#define CW_RADIO_ADMIN_STATE_DISABLED	2

#define CW_RADIO_OPER_STATE_ENABLED	1
#define CW_RADIO_OPER_STATE_DISABLED	2

#define CW_RADIO_OPER_STATE_ENABLED_7	2
#define CW_RADIO_OPER_STATE_DISABLED_7	1


#define CW_RADIO_OPER_CAUSE_NORMAL		0
#define CW_RADIO_OPER_CAUSE_RADIO_FAILURE	1 
#define CW_RADIO_OPER_CAUSE_SW_FAILURE		2
#define CW_RADIO_OPER_CAUSE_ADMIN_SET		3


/**
 * @}
 */





/*
struct cwimage_data {
	uint8_t *data;
	uint8_t type;
	int len;
	uint32_t vendor_id;
	uint8_t *identifier;
};

extern void cw_read_image_data_request(struct cwimage_data *, uint8_t * msg, int len);




extern int cw_send_echo_response(struct conn *conn, int seqnum,
				 struct radioinfo *radioinfo);
extern int cw_handle_echo_request(void *d);
extern void cw_send_image_file(struct conn *conn, FILE * infile);

extern int cw_readmsg_configuration_status_response(uint8_t * elems, int elems_len);
extern int cw_readmsg_configuration_update_request(uint8_t * elems, int elems_len);

*/

/* Message to text stuff */

/* Constants to string conversion lists */
extern struct cw_StrListElem capwap_strings_msg[];
extern struct cw_StrListElem capwap_strings_state[];
extern struct cw_StrListElem capwap_strings_vendor[];
extern struct cw_StrListElem capwap_strings_elem[];
extern struct cw_StrListElem capwap_strings_result[];
extern struct cw_StrListElem capwap_strings_board[];
/*extern struct cw_StrListElem mbag_item_strings[];*/



#define cw_strmsg(id) cw_strlist_get_str(capwap_strings_msg,id)
#define cw_strelem(id) cw_strlist_get_str(capwap_strings_elem,id)


#define cw_strstate(id) cw_strlist_get_str(capwap_strings_state,id)
#define cw_strvendor(id) cw_strlist_get_str(capwap_strings_vendor,id)

#define cw_strresult(id) cw_strlist_get_str(capwap_strings_result,id)

#define cw_strboardelem(id) cw_strlist_get_str(capwap_strings_board,id)

/*#define cw_stritem(id) cw_strlist_get_str(mbag_item_strings,(id))*/

/*
#define cw_strrc(rc) \
	((rc)<0 ? ((rc)!=EAGAIN ? strerror(errno):"Timed out"): cw_strresult(rc))
#define cw_strerror(rc) cw_strrc(rc)
*/

#include "strheap.h"

const char * cw_strerror(int rc);
int cw_result_is_ok(int rc);
const char *cw_strelemp_(cw_strheap_t h, int msg_id);



#define cw_strrc cw_strerror

#define cw_strelemp(p,id) cw_strelemp_((p)->strelem,id)

/*
struct cw_ac_status {
	int stations;
	int limit;
	int active_wtps;
	int max_wtps;
	int security;
	int rmac_field;
	int dtls_policy;
};
*/

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
	CAPWAP_STATE_DISCOVERY,
	/** Join State */
	CAPWAP_STATE_JOIN,
	CAPWAP_STATE_JOIN_COMPLETE,
	
	/** Config State */
	CAPWAP_STATE_CONFIGURE,
	CAPWAP_STATE_DTLS_SETUP, /**< DTLS Setup */
	/** Image Data Upload */
	CW_STATE_IMAGE_DATA,
	CW_STATE_UPDATE,
	/** Run State */
	CAPWAP_STATE_RUN,
	CAPWAP_STATE_DATA_CHECK,
	
	CAPWAP_STATE_TIMEOUT
};

/**
 *@}
 */




#endif
/**
 * @} CAPWAP_DEFS
 */
/**
 * @} CAPWAP
 */
