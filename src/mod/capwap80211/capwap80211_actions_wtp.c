

#include "cw/action.h"
#include "cw/cw.h"
#include "cw/capwap.h"
#include "cw/capwap80211.h"
#include "cw/capwap_items.h"

//#include "capwap_80211_actions.h"
//#include "capwap_80211.h"
//#include "capwap.h"






static cw_action_in_t actions_wtp_in[] = {

	/* --------------------------------------------------------
	 * Discovery Resquest 
	 */

	/* 802.11 Radio Inmformation - Discovery Response */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_RESPONSE, 
		.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION,
		.item_id = "radio_information",
	 	.start = cw_in_radio_generic, 
		.mand = 1, 
		.min_len = 5, 
		.max_len = 5
	}
	,

	/* --------------------------------------------------------
	 * Join Response
	 */

	/* 802.11 Radio Inmformation - Join Response */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_RESPONSE, 
		.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION,
		.item_id = "radio_information",
	 	.start = cw_in_radio_generic, 
		.mand = 1, 
		.min_len = 5, 
		.max_len = 5
	}
	,



//	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST,
//	 CW_ACTION_IN_80211_WTP_RADIO_INFORMATION, 1}
	/* --------------------------------------------------------
	 * Discovery Resquest 
	 */

//	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_REQUEST,
//	 CW_ACTION_IN_80211_WTP_RADIO_INFORMATION, 1}



	{0, 0, 0}
};


cw_action_out_t actions_out[]={
	/* Discovery Request */

	/* 802.11 Radio Information - Discovery Request */
	{
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.item_id = CW_ITEM_RADIO_INFOS ,
	 	.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION, 
		.out = cw_out_radio_infos, 
		.mand = 1
	}
	,

	/* Join  Request */

	/* 802.11 Radio Information - Join Request */
	{
		.msg_id = CW_MSG_JOIN_REQUEST, 
		.item_id = CW_ITEM_RADIO_INFOS ,
	 	.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION, 
		.out = cw_out_radio_infos, 
		.mand = 1
	}
	,




//	{CW_MSG_DISCOVERY_RESPONSE, NULL /*CW_ELEM80211_WTP_RADIO_INFORMATION*/, 0,
//	 CW_ELEM80211_WTP_RADIO_INFORMATION, NULL,cw_out_radio_infos, NULL,1}

	/* Radio Infos */
//	{CW_MSG_JOIN_RESPONSE, NULL /*CW_ELEM80211_WTP_RADIO_INFORMATION*/, 0,
//	 CW_ELEM80211_WTP_RADIO_INFORMATION, NULL,cw_out_radio_infos, NULL,1}

	{0,0,0}
};


int capwap80211_register_actions_wtp(struct cw_actiondef *def)
{

	int rc;
	rc=cw_actionlist_in_register_actions(def->in, actions_wtp_in);
	rc+=cw_actionlist_out_register_actions(def->out, actions_out);

	rc+= cw_strheap_register_strings(def->strelem, capwap_strings_elem80211);
	/*rc += cw_strheap_register_strings(def->strelem, capwap_strings_elem);
*/


	return rc;
}

