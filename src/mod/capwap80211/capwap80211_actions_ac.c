

#include "cw/action.h"
#include "cw/cw.h"
#include "cw/capwap.h"
#include "cw/capwap80211.h"

//#include "capwap_80211_actions.h"
//#include "capwap_80211.h"
//#include "capwap.h"






static cw_action_in_t actions_ac_in[] = {

	/* --------------------------------------------------------
	 * Discovery Resquest 
	 */

	/* 802.11 Radio Inmformation - Discovery Request */
	{
		.capwap_state = CW_STATE_DISCOVERY, 
		.msg_id = CW_MSG_DISCOVERY_REQUEST, 
		.elem_id = CW_ELEM80211_WTP_RADIO_INFORMATION,
		.item_id = "radio_information",
	 	.start = cw_in_radio_generic, 
		.mand = 1, 
		.min_len = 5, 
		.max_len = 5
	}
	,


	/* --------------------------------------------------------
	 * Join Resquest 
	 */

	/* 802.11 Radio Inmformation - Join Request */
	{
		.capwap_state = CW_STATE_JOIN, 
		.msg_id = CW_MSG_JOIN_REQUEST, 
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


cw_action_out_t capwap_80211_actions_ac_out[]={
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
//	rc+=cw_actionlist_out_register_actions(def->out, capwap_80211_actions_ac_out);

	rc+= cw_strheap_register_strings(def->strelem, capwap_strings_elem80211);
	/*rc += cw_strheap_register_strings(def->strelem, capwap_strings_elem);
*/


	return rc;
}

