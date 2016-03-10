#include "capwap.h"

struct cw_strlist_elem capwap_strings_elem[] = {

	{CW_ELEM_AC_DESCRIPTOR, "AC Descriptor"},
	{CW_ELEM_AC_IPV4_LIST, "AC IPv4 List"},
	{CW_ELEM_AC_IPV6_LIST, "AC IPv6 List"},
	{CW_ELEM_AC_NAME, "AC Name"},
	{CW_ELEM_AC_NAME_WITH_PRIORITY, "AC Name with Priority"},
//	{CW_ELEM_AC_NAME_WITH_INDEX, "AC Name with Index"},
	{CW_ELEM_AC_TIMESTAMP, "AC Timestamp"},
	{CW_ELEM_ADD_MAC_ACL_ENTRY, "Add MAC ACL Entry"},
	{CW_ELEM_ADD_STATION, "Add Station"},
	{CW_ELEM_RESERVED_9, "Reserved 9"},
	{CW_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS, "Control IPv4 Address"},
	{CW_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS, "Control IPv6 Address"},
	{CW_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS, "CAWPAP Local IPv4 Address"},
	{CW_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS, "CAPWAP Local IPv6 Address"},
	{CW_ELEM_CAPWAP_TIMERS, "CAPWAP Timers"},
	{CW_ELEM_CAPWAP_TRANSPORT_PROTOCOL, "CAPWAP Transport Protocol"},
	{CW_ELEM_DATA_TRANSFER_DATA, "Data Transfer Data"},
	{CW_ELEM_DATA_TRANSFER_MODE, "Data Transfer Mode"},
	{CW_ELEM_DECRYPTION_ERROR_REPORT, "Encryption Error Report"},
	{CW_ELEM_DECRYPTION_ERROR_REPORT_PERIOD, "Decryption Error Report Period"},
	{CW_ELEM_DELETE_MAC_ACL_ENTRY, "Delete MAC ACL Entry"},
	{CW_ELEM_DELETE_STATION, "Delete Station"},
	{CW_ELEM_RESERVED_19, "Reserved 19"},
	{CW_ELEM_DISCOVERY_TYPE, "Discovery Type"},
	{CW_ELEM_DUPLICATE_IPV4_ADDRESS, "Duplicate IPv4 Address"},
	{CW_ELEM_DUPLICATE_IPV6_ADRESS, "Duplicate IPv6 Address"},
	{CW_ELEM_ECN_SUPPORT, "ECN Support"},
	{CW_ELEM_IDLE_TIMEOUT, "Idle Timeout"},
	{CW_ELEM_IMAGE_DATA, "Image Data"},
	{CW_ELEM_IMAGE_IDENTIFIER, "Image Identifier"},
	{CW_ELEM_IMAGE_INFORMATION, "Image Information"},
	{CW_ELEM_INITIATE_DOWNLOAD, "Initiate Download"},
	{CW_ELEM_LOCATION_DATA, "Location Data"},
	{CW_ELEM_MAXIMUM_MESSAGE_LENGTH, "Maximum Message Length"},
	{CW_ELEM_MTU_DISCOVERY_PADDING, "MTU Discovery Padding"},
	{CW_ELEM_RADIO_ADMINISTRATIVE_STATE, "Radio Administrative State"},
	{CW_ELEM_RADIO_OPERATIONAL_STATE, "Radio Operational State"},
	{CW_ELEM_RESULT_CODE, "Result Code"},
	{CW_ELEM_RETURNED_MESSAGE_ELEMENT, "Returned Message Element"},
	{CW_ELEM_SESSION_ID, "Session ID"},
	{CW_ELEM_STATISTICS_TIMER, "Statistics Timer"},
	{CW_ELEM_VENDOR_SPECIFIC_PAYLOAD, "Vendor Specific Payload"},
	{CW_ELEM_WTP_BOARD_DATA, "WTP Board Data"},
	{CW_ELEM_WTP_DESCRIPTOR, "WTP Descriptor"},
	{CW_ELEM_WTP_FALLBACK, "WTP Fallback"},
	{CW_ELEM_WTP_FRAME_TUNNEL_MODE, "WTP Frame Tunnel Mode"},
	{CW_ELEM_RESERVED_42, "Reserved 42"},
	{CW_ELEM_RESERVED_43, "Reserved 43"},
	{CW_ELEM_WTP_MAC_TYPE, "WTP MAC Type"},
	{CW_ELEM_WTP_NAME, "WTP Name"},
	{CW_ELEM_RESERVED_46, "Reserved 46"},
	{CW_ELEM_WTP_RADIO_STATISTICS, "WTP Radio Statistics"},
	{CW_ELEM_WTP_REBOOT_STATISTICS, "WTP Reboot Statistics"},
	{CW_ELEM_WTP_STATIC_IP_ADDRESS_INFORMATION, "WTP Static IP Address Information"},


/* Cisco's CAPWAP definitions (CAPWAP draft 7) */
/* CW_ELEM_WTP_IPV4_IP_ADDRESS			42
 CW_ELEM_WTP_IPV6_IP_ADDRESS			43
*/
	{CW_STR_STOP,"Unknown Element"}

};
