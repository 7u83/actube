#include "cw/capwap.h"
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
		.min_len = 6,
		.max_len = 6,
		.mand = 1
	}
	,	

	/* Vendor Specific Payload - Discovery Response*/
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM_VENDOR_SPECIFIC_PAYLOAD,
		.start = cw_in_vendor_specific_payload
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
	
	/* ----------------------------------------------------------------
	 * Message Configuration Status Response - IN
	 */
	{
		.capwap_state = CW_STATE_CONFIGURE,
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE,
		.end = cw_in_check_generic_resp
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
		.mand = 0
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


	/* WTP Board Data - Discovery Request */
	{
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id  = CW_ELEM_WTP_BOARD_DATA,
		.item_id = CW_ITEM_WTP_BOARD_DATA, 
		.out = cw_out_wtp_board_data,
		.get = cw_out_get_outgoing,
		.mand = 1
	}
	,


	/* WTP Descriptor */
	{
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id  = CW_ELEM_WTP_DESCRIPTOR,
		.item_id = CW_ITEM_WTP_DESCRIPTOR, 
		.out = capwap_out_wtp_descriptor,
		//.get = cw_out_get_outgoing,
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
		.get = cw_out_get_outgoing,
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


	/* ---------------------------------------------------------------
	 * Configuration Status Request  - Out
	 */

	/* AC Name - Config Status Request */
	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST,
		.elem_id = CW_ELEM_AC_NAME,
		.item_id = CW_ITEM_AC_NAME,
		.out = cw_out_generic,
	       	.get = cw_out_get_local,
		.mand = 1
	}
	,
	/* Radio Admin State - Config Status Request */
	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST,
		.item_id = CW_ITEM_RADIO_ADMINISTRATIVE_STATE,
		.out = cw_out_radio_administrative_states,
	       	.get = cw_out_get_config,
		.mand = 1
	}
	,

//	{CW_MSG_CONFIGURATION_STATUS_REQUEST, CW_ITEM_RADIO_ADMINISTRATIVE_STATE, 0,
//	CW_ELEM_RADIO_ADMINISTRATIVE_STATE, NULL, cw_out_radio_administrative_states, cw_out_get_config,1}



	/* ---------------------------------------------------------------
	 * Change State Event Request  - Out
	 */
	{
		.msg_id = CW_MSG_CHANGE_STATE_EVENT_REQUEST,
		.item_id = CW_ITEM_RADIO_OPER_STATE,
		.out = cw_out_radio_operational_states,
	       	.get = cw_out_get_config,
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
//	rc += cw_itemdefheap_register(def->radioitems,capwap_radioitemdefs);

	return rc;
}
