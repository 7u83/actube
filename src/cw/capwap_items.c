#include "item.h"

#include "capwap_items.h"


const char CW_ITEM_CAPWAP_TRANSPORT_PROTOCOL[]="capwap_transport_protocol";
const char CW_ITEM_IDLE_TIMEOUT[]="idle_timeout";
const char CW_ITEM_DISCOVERY_TYPE[]="discovery_type";
const char CW_ITEM_WTP_NAME[]="wtp_name";
const char CW_ITEM_ECN_SUPPORT[]="ecn_support";
const char CW_ITEM_WTP_FALLBACK[]="wtp_fallback";



const char CW_ITEM_WTP_MAC_TYPE[]="wtp_mac_type";
const char CW_ITEM_WTP_FRAME_TUNNEL_MODE[]="wtp_frame_tunnel_mode";
const char CW_ITEM_WTP_RADIOS_IN_USE[]="max_radios";
const char CW_ITEM_WTP_MAX_RADIOS[]="radios_in_use";

/*
const char CW_ITEM_WTP_HARDWARE_VENDOR=
*/

const char CW_ITEM_WTP_HARDWARE_VERSION[]="wtp_hardware_version";
const char CW_ITEM_WTP_SOFTWARE_VERSION[]="wtp_software_version";
const char CW_ITEM_WTP_BOOT_VERSION[]="wtp_boot_version";
const char CW_ITEM_WTP_OTHER_VERSION[]="wtp_other_version";

/*
const char CW_ITEM_WTP_SOFTWARE_VENDOR,
*/

/*
const char CW_ITEM_WTP_BOOTLOADER_VENDOR,
*/
const char CW_ITEM_WTP_BOOTLOADER_VERSION[]="wtp_bootloader_version";
/*
const char CW_ITEM_WTP_OTHERSOFTWARE_VENDOR,
const char CW_ITEM_WTP_OTHERSOFTWARE_VERSION,
*/
const char CW_ITEM_WTP_BOARD_DATA[]="wtp_board_data";
const char CW_ITEM_WTP_DESCRIPTOR[]="0wtp_descriptor";
const char CW_ITEM_CAPWAP_TIMERS[]="capwap_timers";

const char CW_ITEM_AC_NAME[]="ac_name";
const char CW_ITEM_AC_DESCRIPTOR[]="ac_descriptor";
const char CW_ITEM_RESULT_CODE[]="result_code";
const char CW_ITEM_AC_STATUS[]="ac_status";

const char CW_ITEM_AC_HARDWARE_VERSION[]="ac_hardware_version";
const char CW_ITEM_AC_SOFTWARE_VERSION[]="ac_software_version";

const char CW_ITEM_AC_IP_LIST[]="ac_ip_list";
const char CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST[]="capwap_ctrl_ip_addr";
const char CW_ITEM_CAPWAP_LOCAL_IP_ADDRESS[]="capwap_local_ip_addr";

const char CW_ITEM_LOCATION_DATA[]="location_data";
const char CW_ITEM_SESSION_ID[]="session_id";

const char CW_ITEM_AC_TIMESTAMP[]="ac_timestamp";
const char CW_ITEM_STATISTICS_TIMER[]="statistics_timer";
const char CW_ITEM_WTP_REBOOT_STATISTICS[]="wtp_reboot_statistics";
const char CW_ITEM_IMAGE_IDENTIFIER[]="image_identifier";
//const char CW_ITEM_AC_HASH_VALUE[]="ac_hash_value";

const char CW_ITEM_AC_NAME_WITH_PRIORITY[]="ac_name_with_priority";
const char CW_ITEM_MAXIMUM_MESSAGE_LENGTH[]="maximum_message_length";


	/* CIPWAP and Cisco */
//const char CW_ITEM_WTP_GROUP_NAME[]="wtp_group_name";


	/* Other Items */
const char CW_ITEM_AC_IMAGE_DIR[]="ac_img_dir";		/* Path where WTP images are stored */
const char CW_ITEM_IMAGE_FILENAME[]="img_filename";		/* Full path of image filename */
const char CW_ITEM_DISCOVERIES[]="discoveries";

	/** FILE handle for uploading and downloading images  */
const char CW_ITEM_IMAGE_FILEHANDLE[]="image_file_handle";

const char CW_ITEM_RADIO_CFG[]="radio_cfg";

const char CW_ITEM_CISCO_BOARD_DATA_OPTIONS[]="cisco_board_data_options";
const char CW_ITEM_RADIOS[]="radios";
const char CW_ITEM_RADIO_OPER_STATE[]="radio_oper_state";


const char CW_ITEM_WTP_BOARD_VENDOR[]="vendor_id";
const char CW_ITEM_WTP_BOARD_MODELNO[]="model_no";
const char CW_ITEM_WTP_BOARD_MACADDRESS[]="wtp_board_macaddress";
const char CW_ITEM_WTP_BOARD_ID[]="wtp_board_id";
const char CW_ITEM_WTP_BOARD_REVISION[]="wtp_board_revision";
const char CW_ITEM_WTP_BOARD_SERIALNO[]="serial_no";
const char CW_ITEM_RADIO_INFOS[]="radio_infos";

//const char CW_ITEM_XY[]="wtp_name";


struct cw_itemdef capwap_itemdefs[] = {

	{CW_ITEM_WTP_NAME,CW_ITEM_NONE,MBAG_STR},
	{CW_ITEM_WTP_MAC_TYPE,CW_ITEM_NONE,MBAG_BYTE},
	{CW_ITEM_WTP_FRAME_TUNNEL_MODE,CW_ITEM_NONE,MBAG_BYTE},
	{CW_ITEM_WTP_FALLBACK,CW_ITEM_NONE,MBAG_BYTE},


	/* Reboot Statistics */
	{CW_ITEM_WTP_REBOOT_STATISTICS,CW_ITEM_NONE,MBAG_MBAG},
	{CW_ITEM_WTP_REBOOT_STATISTICS,CW_ITEM_REBOOT_COUNT,MBAG_WORD},
	{CW_ITEM_WTP_REBOOT_STATISTICS,CW_ITEM_REBOOT_AC_INITIATED_COUNT,MBAG_WORD},
	{CW_ITEM_WTP_REBOOT_STATISTICS,CW_ITEM_REBOOT_LINK_FAILURE_COUNT,MBAG_WORD},
	{CW_ITEM_WTP_REBOOT_STATISTICS,CW_ITEM_REBOOT_SW_FAILURE_COUNT,MBAG_WORD},
	{CW_ITEM_WTP_REBOOT_STATISTICS,CW_ITEM_REBOOT_HW_FAILURE_COUNT,MBAG_WORD},
	{CW_ITEM_WTP_REBOOT_STATISTICS,CW_ITEM_REBOOT_OTHER_FAILURE_COUNT,MBAG_WORD},
	{CW_ITEM_WTP_REBOOT_STATISTICS,CW_ITEM_REBOOT_UNKNOWN_FAILURE_COUNT,MBAG_WORD},
	{CW_ITEM_WTP_REBOOT_STATISTICS,CW_ITEM_REBOOT_LAST_FAILURE_TYPE,MBAG_BYTE},





{CW_ITEM_CAPWAP_TRANSPORT_PROTOCOL,CW_ITEM_NONE,MBAG_BYTE},

{CW_ITEM_LOCATION_DATA,CW_ITEM_NONE,MBAG_STR},
{CW_ITEM_WTP_HARDWARE_VERSION,CW_ITEM_NONE,MBAG_VENDORSTR},
{CW_ITEM_WTP_SOFTWARE_VERSION,CW_ITEM_NONE,MBAG_VENDORSTR},
{CW_ITEM_WTP_BOOT_VERSION,CW_ITEM_NONE,MBAG_VENDORSTR},
{CW_ITEM_WTP_OTHER_VERSION,CW_ITEM_NONE,MBAG_VENDORSTR},
//{CW_ITEM_WTP_GROUP_NAME,CW_ITEM_NONE,MBAG_STR},
{CW_ITEM_MAXIMUM_MESSAGE_LENGTH,CW_ITEM_NONE,MBAG_WORD},
{CW_ITEM_STATISTICS_TIMER,CW_ITEM_NONE,MBAG_WORD},
{CW_ITEM_IDLE_TIMEOUT,CW_ITEM_NONE,MBAG_DWORD},
{CW_ITEM_CAPWAP_TIMERS,CW_ITEM_NONE,MBAG_WORD},

{CW_ITEM_AC_NAME_WITH_PRIORITY,CW_ITEM_ANY,MBAG_STR},
{CW_ITEM_AC_NAME,CW_ITEM_NONE,MBAG_STR},
{CW_ITEM_RESULT_CODE,CW_ITEM_NONE,MBAG_DWORD},
{CW_ITEM_ECN_SUPPORT,CW_ITEM_NONE,MBAG_BYTE},
{CW_ITEM_DISCOVERY_TYPE,CW_ITEM_NONE,MBAG_BYTE},
{ CW_ITEM_SESSION_ID,CW_ITEM_NONE,MBAG_BSTR},



//{CW_ITEM_AP_MODE_AND_TYPE,CW_ITEM_NONE,MBAG_WORD},

/* Board Data */
{CW_ITEM_WTP_BOARD_DATA,CW_ITEM_NONE,MBAG_MBAG},
{CW_ITEM_WTP_BOARD_DATA,CW_ITEM_WTP_BOARD_VENDOR,MBAG_DWORD},
{CW_ITEM_WTP_BOARD_DATA,CW_ITEM_WTP_BOARD_MODELNO,MBAG_BSTR16},
{CW_ITEM_WTP_BOARD_DATA,CW_ITEM_WTP_BOARD_SERIALNO,MBAG_BSTR16},


/* Cisco Specific items, should be moved to a file like
cisco_items or so */

{CW_ITEM_CISCO_BOARD_DATA_OPTIONS,CW_ITEM_NONE,MBAG_BSTR16},




{CW_ITEM_NONE}

};


const char CW_RADIOITEM_ADMIN_STATE[]="admin_state";
const char CW_RADIOITEM_OPER_STATE[]="oper_state";
const char CW_RADIOITEM_DECRYPTION_ERROR_REPORT_PERIOD[]="decryption_error_report_period";


struct cw_itemdef capwap_radioitemdefs[] = {
	{CW_RADIOITEM_ADMIN_STATE,CW_ITEM_NONE,MBAG_BYTE},
	{CW_RADIOITEM_OPER_STATE,CW_ITEM_NONE,MBAG_WORD},

	{CW_RADIOITEM_DECRYPTION_ERROR_REPORT_PERIOD,CW_ITEM_NONE,MBAG_WORD},

	{CW_ITEM_NONE}
};
