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


#include "cw/capwap.h"
#include "cw/action.h"
#include "cw/capwap_items.h"
#include "capwap_actions.h"
#include "cw/strheap.h"
#include "cw/radio.h"


#include "mod_capwap.h"

static cw_action_in_t actions_in[] = {


	/* ------------------------------------------------------------------------------- */
	/* Message Discovery Request */
	{
		.capwap_state = CW_STATE_DISCOVERY,
		.msg_id = CW_MSG_DISCOVERY_REQUEST,
		.end = cw_in_check_disc_req
	}
	,

	/* Element Discovery Type */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM_DISCOVERY_TYPE,
	 	.start = cw_in_generic2, 
		.item_id = "discovery_type", 
		.mand = 1, 
		.min_len = 1, 
		.max_len = 1
	}
	,

	/* Element WTP Board Data - Discovery Request */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM_WTP_BOARD_DATA,
	 	.start = capwap_in_wtp_board_data, 
		.item_id = "wtp_board_data", 
		.mand = 1, 
	}
	,

	/* Element WTP Descriptor - Discovery */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM_WTP_DESCRIPTOR,
	 	.start = capwap_in_wtp_descriptor, 
		.item_id = "wtp_descriptor", 
		.mand = 1, 
	}
	,


	/* Element WTP Frame Tunnel Mode */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM_WTP_FRAME_TUNNEL_MODE,
	 	.start = cw_in_generic2, 
		.item_id = "wtp_frame_tunnel_mode", 
		.mand = 1, 
		.min_len = 1, 
		.max_len = 1
	}
	,

	/* Element WTP Mac Type */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM_WTP_MAC_TYPE,
	 	.start = cw_in_generic2, 
		.item_id = "wtp_mac_type", 
		.mand = 1, 
		.min_len = 1, 
		.max_len = 1
	}
	,


	/* Vendor Specific Payload */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM_VENDOR_SPECIFIC_PAYLOAD,
		.start = cw_in_vendor_specific_payload
	}
	,
	


	/* ---------------------------------------------------------------------------
	 * Join Request 
	 */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST,
		.end = cw_in_check_join_req
	}
	,

	/* Location Data - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id =CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_LOCATION_DATA, 
		.item_id = CW_ITEM_LOCATION_DATA,
		.start = cw_in_generic2,
		.max_len = 1024,
		.min_len = 1,
		.mand = 1
	}
	,
	
	/* Element WTP Board Data - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_WTP_BOARD_DATA,
	 	.start = capwap_in_wtp_board_data, 
		.item_id = "wtp_board_data", 
		.mand = 1, 
	}
	,

	/* Element WTP Descriptor - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_WTP_DESCRIPTOR,
	 	.start = capwap_in_wtp_descriptor, 
		.item_id = "wtp_descriptor", 
		.mand = 1, 
	}
	,

	/* Element WTP Name - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_WTP_NAME,
	 	.start = cw_in_generic2, 
		.item_id = "wtp_name", 
		.mand = 1, 
		.min_len = 1, 
		.max_len = 1024
	}
	,

	/* Element Session ID - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_SESSION_ID,
	 	.start = cw_in_generic2, 
		.item_id = CW_ITEM_SESSION_ID, 
		.mand = 1, 
		.min_len = 16, 
		.max_len = 16
	}
	,

	/* WTP Frame Tunnel Mode - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_WTP_FRAME_TUNNEL_MODE,
	 	.start = cw_in_generic2, 
		.item_id = CW_ITEM_WTP_FRAME_TUNNEL_MODE, 
		.mand = 1, 
		.min_len = 1, 
		.max_len = 1
	}
	,

	/* WTP Mac Type - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_WTP_MAC_TYPE,
	 	.start = cw_in_generic2, 
		.item_id = CW_ITEM_WTP_MAC_TYPE, 
		.mand = 1, 
		.min_len = 1, 
		.max_len = 1
	}
	,

	/* Local IPv4 Address - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS,
		.item_id = CW_ITEM_CAPWAP_LOCAL_IP_ADDRESS,
	 	.start = cw_in_capwap_local_ipv4_address, 
		.mand = 1, 
		.min_len = 4, 
		.max_len = 4
	}
	,

	/* Local IPv6 Address - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS,
		.item_id = CW_ITEM_CAPWAP_LOCAL_IP_ADDRESS,
	 	.start = cw_in_capwap_local_ipv4_address, 
		.mand = 1, 
		.min_len = 16, 
		.max_len = 16
	}
	,

	/* ECN Support - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_ECN_SUPPORT,
		.item_id = CW_ITEM_ECN_SUPPORT,
	 	.start = cw_in_generic2, 
		.mand = 1, 
		.min_len = 1, 
		.max_len = 1
	}
	,




	/* Vendor Specific Payload - Join Request*/
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_VENDOR_SPECIFIC_PAYLOAD,
		.start = cw_in_vendor_specific_payload
	}
	,
	
	/* Maximum Message Length - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM_MAXIMUM_MESSAGE_LENGTH,
	 	.start = cw_in_generic2, 
		.item_id = CW_ITEM_MAXIMUM_MESSAGE_LENGTH, 
		.mand = 0, 
		.min_len = 2, 
		.max_len = 2
	}
	,



	/* ---------------------------------------------------------------------------
	 * Configuration Status Request 
	 */
	{
		.capwap_state = CW_STATE_CONFIGURE, 
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST,
//		.end = cw_in_check_join_req
	}
	,




	/* End of list */
	{0, 0}
};

static cw_action_out_t actions_out[]={

	/* ---------------------------------------------------
	 * Discovery Response Message
	 */

	/* Discovery Response AC Descriptor */
	{
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.item_id = CW_ITEM_AC_DESCRIPTOR, 
		.elem_id  = CW_ELEM_AC_DESCRIPTOR,
		.out = capwap_out_ac_descriptor,
		.mand = 1
	}
	,

	/* Discovery Response Elem AC Name */
	{
		.msg_id = CW_MSG_DISCOVERY_RESPONSE,
		.elem_id = CW_ELEM_AC_NAME,
		.item_id = CW_ITEM_AC_NAME,
		.out = cw_out_generic,
	       	.get = cw_out_get_local,
		.mand = 1
	}
	,

	/* List of CAPWAP Control IPv4 and IPv6 addresses 
	 * The handler cw_out_capwap_control_ip_addr_list puts
	 * ipv4 and ipv6 message elements, so the definition
	 * of .elem_id isn't needed.
	 */
	{
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.item_id = CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST,
	 	.out = cw_out_capwap_control_ip_addr_list, 
		.get = cw_out_get_outgoing,
		.mand = 1
	}
	,

	/* ---------------------------------------------------
	 * Join Response Message
	 */

	/* Result Code - Join Response */

	{
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.elem_id = CW_ELEM_RESULT_CODE,
		.item_id = CW_ITEM_RESULT_CODE,
	 	.out = cw_out_generic, 
		.get = cw_out_get_outgoing,
		.mand = 1
	}
	,

	/* ---------------------------------------------------
	 * Configuration Status Response Message
	 */

	{
		.msg_id = CW_MSG_CONFIGURATION_STATUS_RESPONSE, 
		.elem_id = CW_ELEM_IDLE_TIMEOUT,
		.item_id = CW_ITEM_IDLE_TIMEOUT,
	 	.out = cw_out_generic, 
		.get = capwap_out_get_idle_timeout,
		.mand = 1
	}
	,

	


	{0,0}
};


#include "cw/item.h"

/*
static struct cw_itemdef _capwap_itemdefs[] = {

	{"wtp_name",CW_ITEM_NONE,MBAG_STR},
	{"wtp_mac_type",CW_ITEM_NONE,MBAG_BYTE},
	{"discovery_type",CW_ITEM_NONE,MBAG_BYTE},
	{"wtp_frame_tunnel_mode",CW_ITEM_NONE,MBAG_BYTE},
	{ CW_ITEM_LOCATION_DATA, CW_ITEM_NONE,MBAG_STR},
	{ CW_ITEM_SESSION_ID,CW_ITEM_NONE,MBAG_BSTR},
	{CW_ITEM_NONE}

};
*/


int capwap_register_actions_ac(struct cw_actiondef *def)
{
	def->in = cw_actionlist_in_create();
	def->out = cw_actionlist_out_create();
	def->strmsg = cw_strheap_create();
	def->strelem = cw_strheap_create();
	def->wbids = intavltree_create();
	def->items = cw_itemdefheap_create();
	def->radioitems = cw_itemdefheap_create();

	int rc;
	rc = cw_actionlist_in_register_actions(def->in, actions_in);
	rc += cw_actionlist_out_register_actions(def->out, actions_out);

	rc += cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, capwap_strings_elem);

	rc += cw_itemdefheap_register(def->items, capwap_itemdefs);
	rc += cw_itemdefheap_register(def->radioitems, capwap_radioitemdefs);

	intavltree_add(def->wbids, 0);



	return rc;
}
