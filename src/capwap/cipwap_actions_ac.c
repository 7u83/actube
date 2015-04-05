
#include "capwap.h"
#include "capwap_actions.h"



cw_action_in_t cipwap_actions_ac_in[] = {

	/* Element Discovery Type */
	{0, 0, CW_STATE_DISCOVERY, CW_MSG_DISCOVERY_REQUEST, CW_ELEM_DISCOVERY_TYPE,
	 cw_in_generic, 0, CW_ITEMTYPE_BYTE, CW_ITEM_DISCOVERY_TYPE, 4, 4}

};





int cw_register_actions_cipwap_ac(struct cw_actiondef *def)
{
	cw_register_actions_capwap_ac(def);


	cw_actionlist_in_register_actions(def->in, cipwap_actions_ac_in);
//	cw_actionlist_out_register_actions(def->out, capwap_actions_ac_out);

	return 1;
}

