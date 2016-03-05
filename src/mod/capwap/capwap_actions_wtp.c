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
		.start  = cw_in_capwap_control_ipv4_address,
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
		.start  = cw_in_capwap_control_ipv4_address, // Change to v6 handler
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
	

	

};

static cw_action_out_t actions_out[] = {
	/* ---------------------------------------------------------------
	 * Discovery Request 
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
	 * Join Request 
	 */

	/* Discovery Type - Discovery Request */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id  = CW_ELEM_LOCATION_DATA,
		.item_id = CW_ITEM_LOCATION_DATA, 
		.out = cw_out_generic,
		.get = cw_out_get_outgoing,
		.mand = 1
	}
	,





//	{CW_MSG_DISCOVERY_REQUEST, CW_ITEM_DISCOVERY_TYPE, 0,
//	 CW_ELEM_DISCOVERY_TYPE, NULL,cw_out_generic, cw_out_get_outgoing}

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
