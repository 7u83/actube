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


#include "capwap.h"
#include "action.h"
#include "capwap_items.h"
#include "capwap_actions.h"
#include "strheap.h"



cw_action_in_t capwap_actions_ac_in[] = {


	/* -------------------------------------------------------------------------------*/

	/* Message Discovery Request */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, -1,
	 0, cw_in_set_state_none}
	,

	/* Element Discovery Type */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ELEM_DISCOVERY_TYPE,
	 cw_in_generic, 0, CW_ITEMTYPE_BYTE, CW_ITEM_DISCOVERY_TYPE, 1, 1}
	,
	/* Element WTP Board Data */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ACTION_IN_WTP_BOARD_DATA }
	,
	/* Element WTP Descriptor */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ACTION_IN_WTP_DESCRIPTOR }
	,
	/* Element Frame Tunnel Mode */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ACTION_IN_WTP_FRAME_TUNNEL_MODE}
	,
	/* Element WTP Mac Tpe */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ACTION_IN_WTP_MAC_TYPE}
	,
	/* Vendor Specific Payload */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ACTION_IN_VENDOR_SPECIFIC_PAYLOAD}
	,

	/* -------------------------------------------------------------------------------*/

	/* Message: Join Request */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, -1,
	 0, 0}
	,

	/* Element Location Data */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_LOCATION_DATA}
	,
	/* Element WTP Board Data */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_WTP_BOARD_DATA}
	,
	/* Element WTP Board Data */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_WTP_DESCRIPTOR}
	,
	/* Element WTP Board Data */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_WTP_NAME}
	,
	/* Element WTP Board Data */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_SESSION_ID}
	,
	/* Element WTP Board Data */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_WTP_FRAME_TUNNEL_MODE}
	,
	/* Element WTP MAC Type */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_WTP_MAC_TYPE}
	,
	/* Element Vendor Specific Payload */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_VENDOR_SPECIFIC_PAYLOAD}
	,



/*
	{0, 1, 1, CW_MSG_DISCOVERY_RESPONSE, 0,
	 0, 0}
	,


	{0, 1, 1, CW_MSG_DISCOVERY_RESPONSE, CW_ELEM_AC_NAME,
	 cw_out_generic, 0, CW_ITEMTYPE_DATA,CW_ITEM_AC_NAME}
	,

	{0, 1, 2, CW_MSG_DISCOVERY_RESPONSE, 0,
	 0, 0}
	,


*/


/*
	{CW_VENDOR_ID_CISCO, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_CISCO_RAD_NAME,
	 cw_in_wtp_name, 0}
	,
*/

	{0, 0, 0}
};


/*
int cw_out_ac_name(uint8_t * dst, struct cw_item *item)
{

	printf("Putting out the AC name %s\n", item->data);

	uint8_t *data = item->data;
	int len = cw_put_data(dst + 4, data, strlen((char *) data));
	return len + cw_put_elem_hdr(dst, CW_ELEM_AC_NAME, len);
}
*/




cw_action_out_t capwap_actions_ac_out[] = {
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_NONE}
	,
	
	/* AC Descriptor */
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_AC_DESCRIPTOR, 0
	,CW_ELEM_AC_DESCRIPTOR, cw_out_ac_descriptor, 0}
	,

	/* AC Name */
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_AC_NAME, 0,
	CW_ELEM_AC_NAME, cw_out_generic, cw_out_get_local}
	,

	/* List of CAPWAP Control IPv4 and IPv6 addresses */
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_CAPWAP_CONTROL_IP_LIST, 0, 
	0, cw_out_capwap_control_ip_addrs, cw_out_get_local}
	,


	/* -------------------------------------------------------------------------------
	 * Join Response OUT
	 */
	{CW_MSG_JOIN_RESPONSE, CW_ITEM_NONE}
	,

	/* Result Code */	
	{CW_MSG_JOIN_RESPONSE, CW_ITEM_RESULT_CODE, 0,
	CW_ELEM_RESULT_CODE, cw_out_generic, cw_out_get_local}
	,
	
	/* AC Descriptor */
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_AC_DESCRIPTOR, 0
	,CW_ELEM_AC_DESCRIPTOR, cw_out_ac_descriptor, 0}
	,




	{0, 0}

};






int cw_register_actions_capwap_ac(struct cw_actiondef *def)
{
	def->in = cw_actionlist_in_create();
	def->out = cw_actionlist_out_create();
	def->strmsg = cw_strheap_create();
	def->strelem = cw_strheap_create();

	cw_actionlist_in_register_actions(def->in, capwap_actions_ac_in);
	cw_actionlist_out_register_actions(def->out, capwap_actions_ac_out);

	int rc ;
	rc = cw_strheap_register_strings(def->strmsg,capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem,capwap_strings_elem);


	printf("Registered %d strings\n",rc);

	return 1;
}
