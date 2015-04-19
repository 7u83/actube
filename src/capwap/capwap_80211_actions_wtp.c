

#include "action.h"
#include "capwap_80211_actions.h"
#include "capwap_80211.h"
#include "capwap.h"






cw_action_in_t capwap_80211_actions_wtp_in[] = {


	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_RESPONSE,
	 CW_ACTION_IN_80211_WTP_RADIO_INFORMATION, 1}
	,


	{0, 0, 0}
};


cw_action_out_t capwap_80211_actions_wtp_out[]={
	/* Session ID */
	{CW_MSG_JOIN_REQUEST, CW_ELEM80211_WTP_RADIO_INFORMATION, 0,
	 CW_ELEM80211_WTP_RADIO_INFORMATION, NULL,cw_out_radio_infos, NULL,1}
	

};


int cw_register_actions_capwap_80211_wtp(struct cw_actiondef *def)
{

	cw_actionlist_in_register_actions(def->in, capwap_80211_actions_wtp_in);
	cw_actionlist_out_register_actions(def->out, capwap_80211_actions_wtp_out);

	int rc;
	rc = cw_strheap_register_strings(def->strelem, capwap_strings_elem80211);
	printf("80211 Registerd %d\n",rc);
	/*rc += cw_strheap_register_strings(def->strelem, capwap_strings_elem);
*/


	return 1;
}

