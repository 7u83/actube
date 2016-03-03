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

#include "cipwap_actions.h"

cw_action_in_t cipwap_actions_wtp_in[] = {
	


	/* -------------------------------------------------------------------------------
	 * Configuration Update Request IN
	 */
			
	{CW_VENDOR_ID_CISCO, 0, CW_STATE_RUN, CW_MSG_CONFIGURATION_UPDATE_REQUEST, CW_CISCO_RAD_NAME, 
	cw_in_generic, 0, MBAG_BSTR16,CW_ITEM_WTP_NAME,0,512}
	,
	{CW_VENDOR_ID_CISCO, 0, CW_STATE_RUN, CW_MSG_CONFIGURATION_UPDATE_REQUEST, CW_CISCO_LOCATION_DATA, 
	cw_in_generic, 0, MBAG_BSTR16,CW_ITEM_LOCATION_DATA,0,512}
	,

	{CW_VENDOR_ID_CISCO, 0, CW_STATE_RUN, CW_MSG_CONFIGURATION_UPDATE_REQUEST, CW_CISCO_AC_NAME_WITH_INDEX, 
	cw_in_ac_name_with_priority, 0, MBAG_BSTR16,CW_ITEM_AC_NAME_WITH_PRIORITY,0,512}
	,

	/* AP Mode an Type IN */
/*	{CW_VENDOR_ID_CISCO, 0, CW_STATE_RUN, CW_MSG_CONFIGURATION_UPDATE_REQUEST,
	 CW_ACTION_IN_CISCO_AP_MODE_AND_TYPE,
	 1}
	,
*/
	/* Radio Administrative State */
	/* Overload CAPWAP action to handle a radio ID of 255 */
	{0, 0, CW_STATE_RUN, CW_MSG_CONFIGURATION_UPDATE_REQUEST,
	 CW_ACTION_IN_CISCO_RADIO_ADMINISTRATIVE_STATE_WTP, 0}
	,


	/* Add WLAN */
	{CW_VENDOR_ID_CISCO, 0, CW_STATE_RUN, CW_MSG_CONFIGURATION_UPDATE_REQUEST,
	 CW_ACTION_IN_CISCO_ADD_WLAN, 0}
	,



	{CW_VENDOR_ID_CISCO, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST, CW_CISCO_AP_GROUP_NAME, 
	cw_in_generic, 0, MBAG_BSTR16,CW_ITEM_WTP_GROUP_NAME,1,512}
	,


	
	/* -------------------------------------------------------------------------------
	 * Configuration Status Response IN
	 */
	{0, 0, CW_STATE_CONFIGURE, CW_MSG_CONFIGURATION_STATUS_RESPONSE,
	 CW_ACTION_IN_RADIO_ADMINISTRATIVE_STATE_WTP, 0}
	,

	{CW_VENDOR_ID_CISCO, 0, CW_STATE_CONFIGURE, CW_MSG_CONFIGURATION_STATUS_RESPONSE,
	 CW_ACTION_IN_CISCO_RADIO_CFG, 0}
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
	CW_CISCO_AP_GROUP_NAME, NULL,cw_out_generic, cw_out_get_config, 0}
	,

	/* -------------------------------------------------------------------------------
	 * Join Request OUT
	 */

	{CW_MSG_JOIN_REQUEST, CW_ITEM_CISCO_BOARD_DATA_OPTIONS, CW_VENDOR_ID_CISCO,
	CW_CISCO_BOARD_DATA_OPTIONS, NULL,cw_out_generic, cw_out_get_config, 1}
	,
	

	/* -------------------------------------------------------------------------------
	 * Configuration Status Request OUT
	 */


	/* Cisco AP Groupname - Important to get the WTP a DTLS 
	   connection established*/
	{CW_MSG_CONFIGURATION_STATUS_REQUEST, CW_ITEM_RADIO_CFG, CW_VENDOR_ID_CISCO,
	CW_CISCO_AP_GROUP_NAME, NULL,cw_out_cisco_wtp_radio_cfg, cw_out_get_config, 1}
	,

	{CW_MSG_CONFIGURATION_STATUS_REQUEST, CW_ITEM_AP_MODE_AND_TYPE, CW_VENDOR_ID_CISCO,
	CW_CISCO_AP_MODE_AND_TYPE, NULL,cw_out_generic, cw_out_get_config, 1}
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
	int rc;
	rc=cw_register_actions_capwap_wtp(def);


	rc+=cw_actionlist_in_register_actions(def->in, cipwap_actions_wtp_in);
	rc+=cw_actionlist_out_register_actions(def->out, cipwap_actions_wtp_out);


	rc += cw_strheap_register_strings(def->strelem, cipwap_strings_elem);

	return rc;
}
