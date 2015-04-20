/*
    This file is part of libcipwap.

    libcipwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcipwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "cipwap.h"
#include "action.h"
#include "capwap_items.h"
#include "capwap_actions.h"
#include "capwap_cisco.h"
#include "strheap.h"



cw_action_in_t cipwap_actions_wtp_in[] = {
		
	{CW_VENDOR_ID_CISCO, 0, CW_STATE_RUN, CW_MSG_CONFIGURATION_UPDATE_REQUEST, CW_CISCO_RAD_NAME, 
	cw_in_generic, 0, MBAG_BSTR16,CW_ITEM_WTP_NAME,0,512}
	,



	{0, 0, 0}
};




cw_action_out_t cipwap_actions_wtp_out[] = {

	/* -------------------------------------------------------------------------------
	 * Discovery Request OUT
	 */

	/* Cisco AP Groupname - Important to get the WTP a DTLS 
	   connection established*/
	{CW_MSG_DISCOVERY_REQUEST, CW_ITEM_WTP_GROUP_NAME, CW_VENDOR_ID_CISCO,
	CW_CISCO_AP_GROUP_NAME, NULL,cw_out_generic, cw_out_get_local, 0}
	,

	/* -------------------------------------------------------------------------------
	 * Join Request OUT
	 */

	/* Cisco AP Groupname - Important to get the WTP a DTLS 
	   connection established*/
	{CW_MSG_CONFIGURATION_STATUS_REQUEST, CW_ITEM_RADIO_CFG, CW_VENDOR_ID_CISCO,
	CW_CISCO_AP_GROUP_NAME, NULL,cw_out_cisco_wtp_radio_cfg, cw_out_get_local, 1}
	,



//	{CW_MSG_DISCOVERY_REQUEST, CW_ITEM_NONE}
//	,

	/* AC Name */
//	{CW_MSG_DISCOVERY_REQUEST, CW_ITEM_DISCOVERY_TYPE, 0,
//	 CW_ELEM_DISCOVERY_TYPE, NULL,cw_out_generic, cw_out_get_outgoing}
	

	/* -------------------------------------------------------------------------------
	 * Configuration Status Request OUT
	 */
//	{CW_MSG_CONFIGURATION_STATUS_REQUEST, CW_ITEM_NONE}
//	,


	{0, 0}

};





int cw_register_actions_cipwap_wtp(struct cw_actiondef *def)
{
/*
	def->in = cw_actionlist_in_create();
	def->out = cw_actionlist_out_create();
	def->strmsg = cw_strheap_create();
	def->strelem = cw_strheap_create();
*/
	cw_register_actions_capwap_wtp(def);


	cw_actionlist_in_register_actions(def->in, cipwap_actions_wtp_in);
	cw_actionlist_out_register_actions(def->out, cipwap_actions_wtp_out);

	int rc;

	rc += cw_strheap_register_strings(def->strelem, cipwap_strings_elem);



	return 1;
}
