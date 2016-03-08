

#include "action.h"
#include "capwap_80211_actions.h"
#include "capwap80211.h"
#include "capwap.h"






cw_action_in_t capwap_80211_actions_wtp_in[] = {

	/* Discovery Response */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_RESPONSE,
	 CW_ACTION_IN_80211_WTP_RADIO_INFORMATION, 1}
	,

	/* Join Response */
	{0, 0, CW_STATE_JOIN, CW_MSG_JOIN_RESPONSE,
	 CW_ACTION_IN_80211_WTP_RADIO_INFORMATION, 1}
	,


	{0, 0, 0}
};


cw_action_out_t capwap_80211_actions_wtp_out[]={

	/* --------------------------------------------------------
	 * Discovery Resquest 
	 */

	/* 802.11 Radio Information */
	{CW_MSG_DISCOVERY_REQUEST, CW_ITEM_RADIO_INFOS /*CW_ELEM80211_WTP_RADIO_INFORMATION*/, 0,
	 CW_ELEM80211_WTP_RADIO_INFORMATION, NULL,cw_out_radio_infos, NULL,1}
	,

	/* --------------------------------------------------------
	 * Join Resquest 
	 */

	/* 802.11 Radio Information */
	{CW_MSG_JOIN_REQUEST, CW_ITEM_RADIO_INFOS, 0,
	 CW_ELEM80211_WTP_RADIO_INFORMATION, NULL,cw_out_radio_infos, NULL,1},


	{0,0,0}
	

};


int cw_register_actions_capwap_80211_wtp(struct cw_actiondef *def)
{
	int rc;
	rc=cw_actionlist_in_register_actions(def->in, capwap_80211_actions_wtp_in);
	rc+=cw_actionlist_out_register_actions(def->out, capwap_80211_actions_wtp_out);
	rc+=cw_strheap_register_strings(def->strelem, capwap_strings_elem80211);
	return rc;
}

