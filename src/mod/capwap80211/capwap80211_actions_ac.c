

#include "cw/action.h"
#include "cw/cw.h"
#include "cw/capwap.h"
#include "cw/capwap_items.h"

#include "cw/capwap80211.h"
#include "cw/capwap80211_items.h"


//#include "capwap_80211_actions.h"
//#include "capwap_80211.h"
//#include "capwap.h"






static cw_action_in_t actions_ac_in[] = {

	/* --------------------------------------------------------
	 * Discovery Resquest 
	 */

	/* 802.11 Radio Information - Discovery Request */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION,
		.item_id = CW_ITEM80211_WTP_RADIO_INFORMATION,
	 	.start = cw_in_radio_generic, 
		.mand = 1, 
		.min_len = 5, 
		.max_len = 5
	}
	,


	/* --------------------------------------------------------
	 * Join Resquest 
	 */

	/* 802.11 Radio Information - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION,
		.item_id = CW_ITEM80211_WTP_RADIO_INFORMATION,
	 	.start = cw_in_radio_generic, 
		.mand = 1, 
		.min_len = 5, 
		.max_len = 5
	}
	,

	/* --------------------------------------------------------
	 * Configuration Satus Request
	 */

	/* 802.11 Supported Rates - Config Status Request */
	{
		.capwap_state = CW_STATE_CONFIGURE, 
		.msg_id = CW_MSG_CONFIGURATION_STATUS_REQUEST, 
		.elem_id = CW_ELEM80211_SUPPORTED_RATES,
		.item_id = CW_ITEM80211_SUPPORTED_RATES,
	 	.start = cw_in_radio_generic, 
		.mand = 1, 
		.min_len = 3, 
		.max_len = 9
	}
	,





	{0, 0, 0}
};


cw_action_out_t actions_ac_out[]={
	
	/* --------------------------------------------------------
	 * Discovery Response
	 */


	/* 802.11 Radio Information - Discovery Response */
	{
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.item_id = CW_ITEM80211_WTP_RADIO_INFORMATION ,
	 	.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION, 
		.out = cw_out_radio_infos, 
		.mand = 1
	}
	,

	/* --------------------------------------------------------
	 * Join Response
	 */


	/* 802.11 Radio Information - Discovery Response */
	{
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.item_id = CW_ITEM80211_WTP_RADIO_INFORMATION,
	 	.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION, 
		.out = cw_out_radio_infos, 
		.mand = 1
	}
	,


	/* Radio Infos */
//	{CW_MSG_DISCOVERY_RESPONSE, NULL /*CW_ELEM80211_WTP_RADIO_INFORMATION*/, 0,
//	 CW_ELEM80211_WTP_RADIO_INFORMATION, NULL,cw_out_radio_infos, NULL,1}

	/* Radio Infos */
//	{CW_MSG_JOIN_RESPONSE, NULL /*CW_ELEM80211_WTP_RADIO_INFORMATION*/, 0,
//	 CW_ELEM80211_WTP_RADIO_INFORMATION, NULL,cw_out_radio_infos, NULL,1}

	{0,0,0}
};


int capwap80211_register_actions_ac(struct cw_actiondef *def)
{

	int rc;
	rc=cw_actionlist_in_register_actions(def->in, actions_ac_in);
	rc+=cw_actionlist_out_register_actions(def->out, actions_ac_out);

	rc+= cw_strheap_register_strings(def->strelem, capwap_strings_elem80211);

	rc += cw_itemdefheap_register(def->items,capwap80211_itemdefs);

	/*rc += cw_strheap_register_strings(def->strelem, capwap_strings_elem);
*/


	return rc;
}

