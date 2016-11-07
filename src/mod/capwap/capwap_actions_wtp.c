#include "cw/cw.h"
#include "cw/action.h"
#include "cw/capwap_items.h"


#include "mod_capwap.h"


static cw_action_in_t actions_in[] = {

	/* Message Discovery Response */
	{
		.capwap_state = CW_STATE_DISCOVERY,
		.msg_id = CW_MSG_DISCOVERY_RESPONSE,
		.end = cw_in_check_disc_resp
	}
	,

	/* AC Descriptor - Discovery Response */
	{
		.capwap_state = CW_STATE_DISCOVERY,
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.elem_id  = CW_ELEM_AC_DESCRIPTOR,
		.item_id = CW_ITEM_AC_DESCRIPTOR, 
		.start  = cw_in_ac_descriptor,
		.min_len = 12,
		.max_len = 8192,
		.mand = 1
	}
	,

	/* AC Name - Discovery Response */
	{
		.capwap_state = CW_STATE_DISCOVERY,
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.elem_id  = CW_ELEM_AC_NAME,
		.item_id = CW_ITEM_AC_NAME, 
		.start  = cw_in_generic2,
		.min_len = 1,
		.max_len = 512,
		.mand = 1
	}
	,
	
	/* CAPWAP Control IPv4 Address - Discovery Response*/
	{
		.capwap_state = CW_STATE_DISCOVERY,
		.msg_id = CW_MSG_DISCOVERY_RESPONSE,
		.elem_id  = CW_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS,
		.item_id = CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST, 
		.start  = cw_in_capwap_control_ip_address,
		.min_len = 6,
		.max_len = 6,
		.mand = 1
	}

	,
	
	/* CAPWAP Control IPv6 Address - Discovery Response*/
	{
		.capwap_state = CW_STATE_DISCOVERY,
		.msg_id = CW_MSG_DISCOVERY_RESPONSE,
		.elem_id  = CW_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS,
		.item_id = CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST, 
		.start  = cw_in_capwap_control_ip_address, // Change to v6 handler
		.min_len = 18,
		.max_len = 18,
		.mand = 1
	}
	,	

	/* Vendor Specific Payload - Discovery Response*/
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.elem_id = CW_ELEM_VENDOR_SPECIFIC_PAYLOAD,
		.start = cw_in_vendor_specific_payload
	}
	,


	/* Result Code - Discovery Response */
	{
		/*
		 * The Result Code is not defined as part
		 * of Discovery Response
		 */
		.capwap_state = CW_STATE_DISCOVERY,
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.elem_id  = CW_ELEM_RESULT_CODE,
		.item_id = CW_ITEM_RESULT_CODE, 
		.start  = cw_in_generic2,
		.min_len = 4,
		.max_len = 4,
		.mand = 0
	}
		

	,

	/* ----------------------------------------------------------------
	 * Message Join Response 
	 */
	{
		.capwap_state = CW_STATE_JOIN,
		.msg_id = CW_MSG_JOIN_RESPONSE,
		.end = cw_in_check_generic_resp
	}
	,
	/* Result Code - Join Response */
	{
		.capwap_state = CW_STATE_JOIN,
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.elem_id  = CW_ELEM_RESULT_CODE,
		.item_id = CW_ITEM_RESULT_CODE, 
		.start  = cw_in_generic2,
		.min_len = 4,
		.max_len = 4,
		.mand = 1
	}
	,

	/* AC Descriptor - Join Response */
	{
		.capwap_state = CW_STATE_JOIN,
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.elem_id  = CW_ELEM_AC_DESCRIPTOR,
		.item_id = CW_ITEM_AC_DESCRIPTOR, 
		.start  = cw_in_ac_descriptor,
		.min_len = 12,
		.max_len = 8192,
		.mand = 1
	}
	,

	/* AC Name - Join Response */
	{
		.capwap_state = CW_STATE_JOIN,
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.elem_id  = CW_ELEM_AC_NAME,
		.item_id = CW_ITEM_AC_NAME, 
		.start  = cw_in_generic2,
		.min_len = 1,
		.max_len = 512,
		.mand = 1
	}
	,

	/* ECN Support - Join Response */
	{
		.capwap_state = CW_STATE_JOIN,
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.elem_id  = CW_ELEM_ECN_SUPPORT,
		.item_id = CW_ITEM_ECN_SUPPORT, 
		.start  = cw_in_generic2,
		.min_len = 1,
		.max_len = 1,
		.mand = 1
	}
	,

	/* CAPWAP Control IPv4 Address - Join Response*/
	{
		.capwap_state = CW_STATE_JOIN,
		.msg_id = CW_MSG_JOIN_RESPONSE,
		.elem_id  = CW_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS,
		.item_id = CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST, 
		.start  = cw_in_capwap_control_ip_address,
		.min_len = 6,
		.max_len = 6,
		.mand = 1
	}
	,
			
	/* CAPWAP Control IPv6 Address - Join Response*/
	{
		.capwap_state = CW_STATE_JOIN,
		.msg_id = CW_MSG_JOIN_RESPONSE,
		.elem_id  = CW_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS,
		.item_id = CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST, 
		.start  = cw_in_capwap_control_ip_address, // Change to v6 handler
		.min_len = 18,
		.max_len = 18,
		.mand = 1
	}
	,	


	/* Maximum Message Length - Join Response */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.elem_id = CW_ELEM_MAXIMUM_MESSAGE_LENGTH,
	 	.start = cw_in_generic2, 
		.item_id = CW_ITEM_MAXIMUM_MESSAGE_LENGTH, 
		.min_len = 2, 
		.max_len = 2
	}
	,

	
	/* ----------------------------------------------------------------
	 * Message Configuration Status Response - IN
	 */
	{
		.capwap_state = CW_STATE_CONFIGURE,
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE,
		.end = cw_in_check_generic_resp
	}
	,

		
	/* Capwap Timers - Config Status Resp */
	{
		.capwap_state = CW_STATE_CONFIGURE,
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE, 
		.elem_id  = CW_ELEM_CAPWAP_TIMERS,
		.item_id = CW_ITEM_CAPWAP_TIMERS, 
		.start  = cw_in_generic2,
		.min_len = 2,
		.max_len = 2,
		.mand = 1
	}
	,

	/* Decryption Error Report - Config Status Resp */
	{
		.capwap_state = CW_STATE_CONFIGURE,
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE, 
		.elem_id  = CW_ELEM_DECRYPTION_ERROR_REPORT_PERIOD,
		.item_id = CW_RADIOITEM_DECRYPTION_ERROR_REPORT_PERIOD, 
		.start  = cw_in_radio_generic,
		.min_len = 3,
		.max_len = 3,
		.mand = 1
	}
	,

	
	/* Idle Timeout - Config Status Resp */
	{
		.capwap_state = CW_STATE_CONFIGURE,
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE, 
		.elem_id  = CW_ELEM_IDLE_TIMEOUT,
		.item_id = CW_ITEM_IDLE_TIMEOUT, 
		.start  = cw_in_generic2,
		.min_len = 4,
		.max_len = 4,
		.mand = 1
	}
	,


	/* WTP Fallback - Config Status Resp */
	{
		.capwap_state = CW_STATE_CONFIGURE,
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE, 
		.elem_id  = CW_ELEM_WTP_FALLBACK,
		.item_id = CW_ITEM_WTP_FALLBACK, 
		.start  = cw_in_generic2,
		.min_len = 1,
		.max_len = 1,
		.mand = 1
	}
	,




	/* Result Code - Config Status Resp */
	{
		.capwap_state = CW_STATE_CONFIGURE,
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE, 
		.elem_id  = CW_ELEM_RESULT_CODE,
		.item_id = CW_ITEM_RESULT_CODE, 
		.start  = cw_in_generic2,
		.min_len = 4,
		.max_len = 4,
		.mand = 0
	}
	,


	/* AC IPv4 List - Config Status Resp */
	{
		.capwap_state = CW_STATE_CONFIGURE,
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE,
		.elem_id = CW_ELEM_AC_IPV4_LIST,
		.item_id = CW_ITEM_AC_IP_LIST,
		.start = NULL,
		.min_len = 4,
		.max_len = 1024*4,
		.mand = 0
	}	
	,
	

	/* Vendor Specific Payload - Cponfig Status Resp */
	{
		.capwap_state = CW_STATE_CONFIGURE, 
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE, 
		.elem_id = CW_ELEM_VENDOR_SPECIFIC_PAYLOAD,
		.start = cw_in_vendor_specific_payload
	}
	,



	
	/* ----------------------------------------------------------------
	 * Change State Event Response - IN
	 */

	{
		.capwap_state = CW_STATE_CONFIGURE, 
		.msg_id = CW_MSG_CHANGE_STATE_EVENT_RESPONSE,
	 	.end =	cw_in_check_cfg_update_req  
	}
	,

	{
		.capwap_state = CW_STATE_CONFIGURE,
		.msg_id = CW_MSG_CHANGE_STATE_EVENT_RESPONSE, 
		.elem_id  = CW_ELEM_RESULT_CODE,
		.item_id = CW_ITEM_RESULT_CODE, 
		.start  = cw_in_generic2,
		.min_len = 4,
		.max_len = 4,
		.mand = 1
	}
	,

	/* ----------------------------------------------------------------
	 * Change State Event Response - IN Run State
	 */

	{
		.capwap_state = CW_STATE_RUN, 
		.msg_id = CW_MSG_CHANGE_STATE_EVENT_RESPONSE,
	 	.end =	cw_in_check_cfg_update_req  
	}
	,

	{
		.capwap_state = CW_STATE_RUN,
		.msg_id = CW_MSG_CHANGE_STATE_EVENT_RESPONSE, 
		.elem_id  = CW_ELEM_RESULT_CODE,
		.item_id = CW_ITEM_RESULT_CODE, 
		.start  = cw_in_generic2,
		.min_len = 4,
		.max_len = 4,
		.mand = 1
	}
	,




		
	/* ----------------------------------------------------------------
	 * Message Echo Response - IN
	 */
	{
		.capwap_state = CW_STATE_RUN,
		.msg_id = CW_MSG_ECHO_RESPONSE,
	}
	,

	/* Vendor Specific Payload - Echo Response */
	{
		.capwap_state = CW_STATE_RUN, 
		.msg_id = CW_MSG_ECHO_RESPONSE, 
		.elem_id = CW_ELEM_VENDOR_SPECIFIC_PAYLOAD,
		.start = cw_in_vendor_specific_payload
	}
	,



	/* ----------------------------------------------------------------
	 * Configuration Update Request  - IN
	 */
	{
		.capwap_state = CW_STATE_RUN,
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST,
	}
	,


	/* WTP Name - Config Update Req */
	{
		.capwap_state = CW_STATE_RUN,
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST, 
		.elem_id  = CW_ELEM_WTP_NAME,
		.item_id = CW_ITEM_WTP_NAME, 
		.start  = cw_in_generic2,
		.min_len = 1,
		.max_len = 1024,
		.mand = 0
	}
	,




	/* Location Data - Config Update Req */
	{
		.capwap_state = CW_STATE_RUN,
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST, 
		.elem_id  = CW_ELEM_LOCATION_DATA,
		.item_id = CW_ITEM_LOCATION_DATA, 
		.start  = cw_in_generic2,
		.min_len = 1,
		.max_len = 1024,
		.mand = 0
	}
	,


	/*  Radio Admin State - Config Update Request */
	{
		.capwap_state = CW_STATE_RUN, 
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST,
		.elem_id = CW_ELEM_RADIO_ADMINISTRATIVE_STATE,
		.item_id = CW_RADIOITEM_ADMIN_STATE,
		.start = cw_in_radio_generic,
		.min_len=2,
		.max_len=2,
		.mand = 0
	}
	,


	/* Vendor Specific Payload - Config Update Req */
	{
		.capwap_state = CW_STATE_RUN, 
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_REQUEST, 
		.elem_id = CW_ELEM_VENDOR_SPECIFIC_PAYLOAD,
		.start = cw_in_vendor_specific_payload
	}
	,
		
	
};

static cw_action_out_t actions_out[] = {
	/* ---------------------------------------------------------------
	 * Discovery Request - Out
	 */

	/* Discovery Type - Discovery Request */
	{
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id  = CW_ELEM_DISCOVERY_TYPE,
		.item_id = CW_ITEM_DISCOVERY_TYPE, 
		.out = cw_out_generic,
		.get = cw_out_get_outgoing,
		.mand = 1
	}
	,

	/* WTP Board Data - Discovery Request */
	{
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id  = CW_ELEM_WTP_BOARD_DATA,
		.item_id = CW_ITEM_WTP_BOARD_DATA, 
		.out = cw_out_wtp_board_data,
		.get = cw_out_get_config,
		.mand = 1
	}
	,

	/* WTP Descriptor - Discover Request */
	{
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id  = CW_ELEM_WTP_DESCRIPTOR,
		.item_id = CW_ITEM_WTP_DESCRIPTOR, 
		.out = capwap_out_wtp_descriptor,
		//.get = cw_out_get_config,
		.mand = 1
	}

	,

	/* WTP Frame TunnelMode - Discovery Request */
	{
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id  = CW_ELEM_WTP_FRAME_TUNNEL_MODE,
		.item_id = CW_ITEM_WTP_FRAME_TUNNEL_MODE, 
		.out = cw_out_generic,
		.get = cw_out_get_local,
		.mand = 1
	}
	,

	/* WTP Mac Type - Discovery Request */
	{
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id  = CW_ELEM_WTP_MAC_TYPE,
		.item_id = CW_ITEM_WTP_MAC_TYPE, 
		.out = cw_out_generic,
		.get = cw_out_get_local,
		.mand = 1
	}
	,


	/* ---------------------------------------------------------------
	 * Join Request - Out
	 */

	/* Location Data - Join Request  */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_LOCATION_DATA,
		.item_id = CW_ITEM_LOCATION_DATA, 
		.out = cw_out_generic,
		.get = cw_out_get_config,
		.mand = 1
	}
	,

	/* WTP Board Data - Join Request  */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_WTP_BOARD_DATA,
		.item_id = CW_ITEM_WTP_BOARD_DATA, 
		.out = cw_out_wtp_board_data,
		.get = cw_out_get_config,
		.mand = 1
	}
	,

	/* WTP Descriptor - Join Request  */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_WTP_DESCRIPTOR,
		.item_id = CW_ITEM_WTP_DESCRIPTOR, 
		.out = cw_out_wtp_descriptor,
		.mand = 1
	}
	,

	/* WTP Name - Join Request  */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_WTP_NAME,
		.item_id = CW_ITEM_WTP_NAME, 
		.out = cw_out_generic,
		.get = cw_out_get_config,
		.mand = 1
	}
	,

	/* Session ID - Join Request  */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_SESSION_ID,
		.item_id = CW_ITEM_SESSION_ID, 
		.out = cw_out_generic,
		.get = cw_out_get_session_id,
		.mand = 1
	}
	,

	/* WTP Frame Tunnel Mode - Join Request  */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_WTP_FRAME_TUNNEL_MODE,
		.item_id = CW_ITEM_WTP_FRAME_TUNNEL_MODE, 
		.out = cw_out_generic,
		.get = cw_out_get_local,
		.mand = 1
	}
	,


	/* WTP MAC Type - Join Request  */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_WTP_MAC_TYPE,
		.item_id = CW_ITEM_WTP_MAC_TYPE, 
		.out = cw_out_generic,
		.get = cw_out_get_local,
		.mand = 1
	}
	,

	/* ECN Support - Join Request */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_ECN_SUPPORT,
		.item_id = CW_ITEM_ECN_SUPPORT, 
		.out = cw_out_generic,
		.get = cw_out_get_config,
		.mand = 1
	}
	,

	/* CAPWAP Local IP Address - Join Request */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.item_id = CW_ITEM_CAPWAP_LOCAL_IP_ADDRESS, 
		.out = cw_out_capwap_local_ip_address,
		.mand = 1
	}
	,

	/* WTP Reboot Statistics - Join Request */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_WTP_REBOOT_STATISTICS,
		.item_id = CW_ITEM_WTP_REBOOT_STATISTICS, 
		.out = cw_out_wtp_reboot_statistics,
		.get = cw_out_get_config,
		.mand = 0
	}
	,





	/* ---------------------------------------------------------------
	 * Configuration Status Request  - Out
	 */

	/* AC Name - Config Status Request - OUT */
	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST,
		.elem_id = CW_ELEM_AC_NAME,
		.item_id = CW_ITEM_AC_NAME,
		.out = cw_out_generic,
	       	.get = cw_out_get_local,
		.mand = 1
	}
	,

	/* Radio Admin State - Config Status Request - OUT */
	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST,
		.elem_id = CW_ELEM_RADIO_ADMINISTRATIVE_STATE,
		.item_id = CW_RADIOITEM_ADMIN_STATE,
		.out = cw_out_radio_generic,
//		.out = cw_out_radio_administrative_states,
//	       	.get = cw_out_get_config,
		.mand = 1
	}
	,



	/* WTP Reboot Statistics - Config Status Request */
	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST, 
		.elem_id = CW_ELEM_WTP_REBOOT_STATISTICS,
		.item_id = CW_ITEM_WTP_REBOOT_STATISTICS, 
		.out = cw_out_wtp_reboot_statistics,
		.get = cw_out_get_config,
		.mand = 1
	}
	,


	/* Statistics Timer - Join Request */
	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST, 
		.elem_id  = CW_ELEM_STATISTICS_TIMER,
		.item_id = CW_ITEM_STATISTICS_TIMER, 
		.out = cw_out_generic,
		.get = cw_out_get_config,
		.mand = 1
	}
	,





	/* ---------------------------------------------------------------
	 * Change State Event Request  - Out
	 */

	/* Radio Operational State(s) - Change State Event */
	{
		.msg_id = CW_MSG_CHANGE_STATE_EVENT_REQUEST,
		.elem_id = CW_ELEM_RADIO_OPERATIONAL_STATE,
		.item_id = CW_RADIOITEM_OPER_STATE,
//		.out = cw_out_radio_operational_state,
		.out = cw_out_radio_generic,		
		.mand = 1
	}
	,


	{
		.msg_id = CW_MSG_CHANGE_STATE_EVENT_REQUEST,
		.elem_id = CW_ELEM_RESULT_CODE,
		.item_id = CW_ITEM_RESULT_CODE,
		.out = cw_out_generic,
	       	.get = cw_out_get_outgoing,
		.mand = 1
	}
	,


	/* ---------------------------------------------------------------
	 * Echo Request - Out
	 */

	/* Echo Request  */
	{
		.msg_id = CW_MSG_ECHO_REQUEST, 
	}
	,




	/* ---------------------------------------------------------------
	 * Configuration Update REsponse  - Out
	 */
	{
		.msg_id = CW_MSG_CONFIGURATION_UPDATE_RESPONSE,
		.mand = 1
	}
	,



	/* Radio Ooerational State */
//	{CW_MSG_CHANGE_STATE_EVENT_REQUEST, CW_ITEM_RADIO_OPER_STATE, 0,
//	0, NULL, cw_out_radio_operational_states, cw_out_get_config,1}
//	,



	/* WTP Descriptor */
//	{CW_MSG_JOIN_REQUEST, CW_ITEM_WTP_DESCRIPTOR, 0, 
//	 CW_ELEM_WTP_DESCRIPTOR, NULL,cw_out_wtp_descriptor, NULL,1}
//	,
	
};


int capwap_register_actions_wtp(struct cw_actiondef *def)
{
	def->in = cw_actionlist_in_create();
	def->out = cw_actionlist_out_create();
	def->strmsg = cw_strheap_create();
	def->strelem = cw_strheap_create();
	def->items = cw_itemdefheap_create();
	def->radioitems = cw_itemdefheap_create();



	int rc;

	rc = cw_actionlist_in_register_actions(def->in, actions_in);
	rc += cw_actionlist_out_register_actions(def->out, actions_out);

	rc += cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, capwap_strings_elem);

	rc += cw_itemdefheap_register(def->items,capwap_itemdefs);
	rc += cw_itemdefheap_register(def->radioitems,capwap_radioitemdefs);

	return rc;
}
