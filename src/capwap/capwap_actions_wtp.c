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



cw_action_in_t capwap_actions_wtp_in[] = {


	/* ------------------------------------------------------------------------------- */

	/* Message Discovery Response */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_RESPONSE, 0,
	 0, cw_in_check_disc_resp}
	,
	/* AC Descriptor */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_RESPONSE,
	 CW_ACTION_IN_AC_DESCRIPTOR, 1}
	,

	/* Element: AC Name */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_RESPONSE,
	 CW_ACTION_IN_AC_NAME, 1}
	,

	/* Element CAPWAP Control IPv4 Address */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_RESPONSE,
	 CW_ACTION_IN_CAPWAP_CONTROL_IPV4_ADDRESS, 1}
	,


	/* Element: Vendor Specific */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_RESPONSE,
	 CW_ACTION_IN_VENDOR_SPECIFIC_PAYLOAD, 0}
	,


	/* ------------------------------------------------------------------------------- */

	/* Message Join  Response */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_RESPONSE, 0,
	 0, cw_in_check_join_resp }
	,

	/* Element: Vendor Specific */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_RESPONSE,
	 CW_ACTION_IN_RESULT_CODE, 1}
	,

		


	{0, 0, 0}
};




cw_action_out_t capwap_actions_wtp_out[] = {

	/* -------------------------------------------------------------------------------
	 * Discovery Request OUT
	 */


	{CW_MSG_DISCOVERY_REQUEST, CW_ITEM_NONE}
	,

	/* AC Name */
	{CW_MSG_DISCOVERY_REQUEST, CW_ITEM_DISCOVERY_TYPE, 0,
	 CW_ELEM_DISCOVERY_TYPE, cw_out_generic, cw_out_get_outgoing}
	,


	/* WTP Board Data */
	{CW_MSG_DISCOVERY_REQUEST, CW_ITEM_WTP_BOARD_DATA, 0,
	 CW_ELEM_WTP_BOARD_DATA, cw_out_wtp_board_data, cw_out_get_outgoing}
	,


	/* -------------------------------------------------------------------------------
	 * Join Request OUT
	 */

	{CW_MSG_JOIN_REQUEST, CW_ITEM_NONE}
	,

	/* Location Data */
	{CW_MSG_JOIN_REQUEST, CW_ITEM_LOCATION_DATA, 0,
	 CW_ELEM_LOCATION_DATA, cw_out_generic, cw_out_get_local,1}
	,

	/* WTP Name */
	{CW_MSG_JOIN_REQUEST, CW_ITEM_WTP_NAME, 0,
	 CW_ELEM_WTP_NAME, cw_out_generic, cw_out_get_local,1}
	,


	{0, 0}

};





int cw_register_actions_capwap_wtp(struct cw_actiondef *def)
{
	def->in = cw_actionlist_in_create();
	def->out = cw_actionlist_out_create();
	def->strmsg = cw_strheap_create();
	def->strelem = cw_strheap_create();

	cw_actionlist_in_register_actions(def->in, capwap_actions_wtp_in);
	cw_actionlist_out_register_actions(def->out, capwap_actions_wtp_out);

	int rc;
	rc = cw_strheap_register_strings(def->strmsg, capwap_strings_msg);
	rc += cw_strheap_register_strings(def->strelem, capwap_strings_elem);


	printf("Registered %d strings\n", rc);

	return 1;
}
