#include "capwap/capwap.h"
#include "capwap/action.h"
#include "capwap/capwap_items.h"


static cw_action_in_t actions_in[] = {



	

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
		.out = cw_out_wtp_descriptor,
		//.get = cw_out_get_outgoing,
		.mand = 1
	}






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
