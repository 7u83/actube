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
 * @brief Definitions for CAPWAP 
 */



#ifndef __CAPWAPMSG_H
#define __CAPWAPMSG_H

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




#endif
