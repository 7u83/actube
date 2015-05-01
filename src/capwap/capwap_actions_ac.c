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


	/* ------------------------------------------------------------------------------- */

	/* Message Discovery Request */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, 0,
	 0, cw_in_check_disc_req}
	,

	/* Element Discovery Type */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ELEM_DISCOVERY_TYPE,
	 cw_in_generic, 0, MBAG_BYTE, CW_ITEM_DISCOVERY_TYPE, 1, 1, 1}
	,
	/* Element Frame Tunnel Mode */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST,
	 CW_ACTION_IN_WTP_FRAME_TUNNEL_MODE, 1}
	,
	/* Element WTP Mac Tpe */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ACTION_IN_WTP_MAC_TYPE, 1}
	,
	/* Vendor Specific Payload */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST,
	 CW_ACTION_IN_VENDOR_SPECIFIC_PAYLOAD}
	,
	/* Element WTP Descriptor */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ACTION_IN_WTP_DESCRIPTOR,
	 1}
	,
	/* Element WTP Board Data */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ACTION_IN_WTP_BOARD_DATA,
	 1}
	,

	/* Element WTP Board Data */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_DISCOVERY_REQUEST, CW_ACTION_IN_WTP_BOARD_DATA,
	 1}
	,


	/* ------------------------------------------------------------------------------- */

	/* Message: Join Request */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, 0,
	 0, cw_in_check_join_req}
	,

	/* Location Data */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_LOCATION_DATA, 1}
	,
	/* WTP Board Data */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_WTP_BOARD_DATA, 1}
	,
	/* WTP Board Descriptor */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_WTP_DESCRIPTOR, 1}
	,
	/* Element WTP Name */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_WTP_NAME, 1}
	,
	/* Element Session ID */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_SESSION_ID, 1}
	,
	/* Element WTP Frame Tunnel Mode */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_WTP_FRAME_TUNNEL_MODE, 1}
	,
	/* Element WTP MAC Type */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_WTP_MAC_TYPE, 1}
	,
	/* Element Vendor Specific Payload */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_ACTION_IN_VENDOR_SPECIFIC_PAYLOAD}
	,

	/* ------------------------------------------------------------------------------- */

	/* Message: Configuration Status Request */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_CONFIGURATION_STATUS_REQUEST, 0,
	 0, 0}
	,
	/* Element: AC Name */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_CONFIGURATION_STATUS_REQUEST,
	 CW_ACTION_IN_AC_NAME, 1}
	,
	/* Element: Statistics Timer */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_CONFIGURATION_STATUS_REQUEST,
	 CW_ACTION_IN_STATISTICS_TIMER, 1}
	,

	/* Element: Statistics Timer */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_CONFIGURATION_STATUS_REQUEST,
	 CW_ACTION_IN_WTP_REBOOT_STATISTICS, 1}
	,

	/* Element Vendor Specific Payload */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_CONFIGURATION_STATUS_REQUEST,
	 CW_ACTION_IN_VENDOR_SPECIFIC_PAYLOAD}
	,


	/* ------------------------------------------------------------------------------- */

	/* Message: Image Data Request - in Config State */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_IMAGE_DATA_REQUEST, 0,
	 0, cw_in_check_img_data_req_ac}
	,
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_IMAGE_DATA_REQUEST,
	 CW_ACTION_IN_IMAGE_IDENTIFIER,
	 0}
	,


	/* ------------------------------------------------------------------------------- */

	/* Message: Change State Event Request  - in Config State */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_CHANGE_STATE_EVENT_REQUEST, 0,
	 cw_in_check_chng_state_evnt_req, 0}
	,

	/* Element: Result Code */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_CHANGE_STATE_EVENT_REQUEST,
	 CW_ACTION_IN_RESULT_CODE, 1}
	,


	/* ------------------------------------------------------------------------------- */

	/* Message: Change State Event Request  - in Config State */
	{0, 0, CW_STATE_RUN, CW_MSG_CHANGE_STATE_EVENT_REQUEST, 0,
	 cw_in_check_chng_state_evnt_req, 0}
	,







	/* ------------------------------------------------------------------------------- */

	/* Message Image Data Response IN */
	{0, 0, CW_STATE_IMAGE_DATA, CW_MSG_IMAGE_DATA_RESPONSE, 0,
	 0, cw_in_check_img_data_resp }
	,

	/* Element: Result Code */
	{0, 0, CW_STATE_IMAGE_DATA, CW_MSG_IMAGE_DATA_RESPONSE,
	 CW_ACTION_IN_RESULT_CODE, 1}

	,


	/* ------------------------------------------------------------------------------- */
	
	/*  WTP Event Request */
	{0, 0, CW_STATE_RUN, CW_MSG_WTP_EVENT_REQUEST, 0,
	 0, 0 }
	,
	/* Vendor Specific Payload */
	{0, 0, CW_STATE_RUN, CW_MSG_WTP_EVENT_REQUEST,
	 CW_ACTION_IN_VENDOR_SPECIFIC_PAYLOAD}
	,



	/* ------------------------------------------------------------------------------- */
	
	/*  Echoh Request */
	{0, 0, CW_STATE_RUN, CW_MSG_ECHO_REQUEST, 0,
	 0, 0 }
	,


	/* Message: Join Request */
	{0, 0, CW_STATE_RUN, CW_MSG_CONFIGURATION_UPDATE_RESPONSE, 0,
	 0, 0}
	,



	{0, 0, 0}
};



cw_action_out_t capwap_actions_ac_out[] = {
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_NONE}
	,

	/* AC Descriptor */
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_AC_DESCRIPTOR, 0, CW_ELEM_AC_DESCRIPTOR,
	 NULL,cw_out_ac_descriptor, 0}
	,

	/* AC Name */
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_AC_NAME, 0,
	CW_ELEM_AC_NAME, NULL, cw_out_generic, cw_out_get_local}
	,

	/* List of CAPWAP Control IPv4 and IPv6 addresses */
	{CW_MSG_DISCOVERY_RESPONSE, CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST, 0,
	 0, NULL, cw_out_capwap_control_ip_addr_list, cw_out_get_outgoing}
	,


	/* -------------------------------------------------------------------------------
	 * Join Response OUT
	 */
	{CW_MSG_JOIN_RESPONSE, CW_ITEM_NONE}
	,

	/* Result Code */
	{CW_MSG_JOIN_RESPONSE, CW_ITEM_RESULT_CODE, 0,
	 CW_ELEM_RESULT_CODE, NULL,cw_out_generic, cw_out_get_outgoing, 1}
	,

	/* AC Descriptor */
	{CW_MSG_JOIN_RESPONSE, CW_ITEM_AC_DESCRIPTOR, 0, CW_ELEM_AC_DESCRIPTOR,
	 NULL,cw_out_ac_descriptor, 0, 1}
	,

	/* AC Name */
	{CW_MSG_JOIN_RESPONSE, CW_ITEM_AC_NAME, 0,
	 CW_ELEM_AC_NAME, NULL, cw_out_generic, cw_out_get_local,1}
	,

	/* Local IP Address */
	{CW_MSG_JOIN_RESPONSE, CW_ITEM_CAPWAP_LOCAL_IP_ADDRESS, 0,
	 0, NULL, cw_out_capwap_local_ip_address, NULL,1}
	,

	/* List of CAPWAP Control IPv4 and IPv6 addresses */
	{CW_MSG_JOIN_RESPONSE, CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST, 0,
	 0, NULL, cw_out_capwap_control_ip_addr_list, cw_out_get_outgoing}
	,




	/* -------------------------------------------------------------------------------
	 * Image Data Response OUT
	 */
	{CW_MSG_IMAGE_DATA_RESPONSE, CW_ITEM_NONE}
	,

	/* Result Code */
	{CW_MSG_IMAGE_DATA_RESPONSE, CW_ITEM_RESULT_CODE, 0,
	 CW_ELEM_RESULT_CODE, NULL,cw_out_generic, cw_out_get_outgoing, 1}
	,


	/* -------------------------------------------------------------------------------
	 * Image Data Response OUT
	 */
	{CW_MSG_CONFIGURATION_STATUS_RESPONSE, CW_ITEM_NONE}
	,

	/* Result Code */
//	{CW_MSG_CONFIGURATION_STATUS_RESPONSE, CW_ITEM_RESULT_CODE, 0,
//	 CW_ELEM_RESULT_CODE, NULL,cw_out_generic, cw_out_get_outgoing, 1}
//	,





	/* -------------------------------------------------------------------------------
	 * Image Data Request OUT
	 */

	{CW_MSG_IMAGE_DATA_REQUEST, CW_ITEM_NONE}
	,
	/* Image Data */
	{CW_MSG_IMAGE_DATA_REQUEST, CW_ITEM_IMAGE_FILEHANDLE, 0,
	 CW_ELEM_IMAGE_DATA, NULL,cw_out_image_data, cw_out_get_outgoing,0}
	,


	/* -------------------------------------------------------------------------------
	 * Change State Event Response OUT
	 */

	{CW_MSG_CHANGE_STATE_EVENT_RESPONSE, CW_ITEM_NONE}
	,	


	/* -------------------------------------------------------------------------------
	 * WTP Event Response OUT
	 */

	{CW_MSG_WTP_EVENT_RESPONSE, CW_ITEM_NONE}
	,	


	/* -------------------------------------------------------------------------------
	 * Echo Response
	 */

	{CW_MSG_ECHO_RESPONSE, CW_ITEM_NONE}
	,	


	/* -------------------------------------------------------------------------------
	 * Update Request
	 */
	{CW_MSG_CONFIGURATION_UPDATE_REQUEST, CW_ITEM_NONE}
	,

	/* Result Code */
	{CW_MSG_CONFIGURATION_UPDATE_REQUEST, CW_ITEM_WTP_NAME, 0,
	 CW_ELEM_WTP_NAME, NULL,cw_out_generic, cw_out_get_outgoing, 1}
	,





	/* End of list */
	{0, 0}

};


#include "item.h"



int cw_register_actions_capwap_ac(struct cw_actiondef *def)
{
	def->in = cw_actionlist_in_create();
	def->out = cw_actionlist_out_create();
	def->strmsg = cw_strheap_create();
	def->strelem = cw_strheap_create();
	def->wbids = intavltree_create();
	def->items = cw_itemdefheap_create();

	int rc;
	rc = cw_actionlist_in_register_actions(def->in, capwap_actions_ac_in);
	rc += cw_actionlist_out_register_actions(def->out, capwap_actions_ac_out);

	rc += cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, capwap_strings_elem);

	rc += cw_itemdefheap_register(def->items,capwap_itemdefs);
	
	
	intavltree_add(def->wbids,0);



	return rc;
}
