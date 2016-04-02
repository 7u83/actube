

#include "../modload.h"

#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "mod_cipwap.h"



int cipwap_init()
{
	return 0;
}


static int detect(struct conn *conn,const uint8_t *rawmsg, int rawlen,int elems_len, struct sockaddr *from, int mode)
{
	if (mode == MOD_MODE_CAPWAP)
		return 1;

	return 0;
}

static int register_actions(struct cw_actiondef *actions, int mode)
{
	switch (mode) {
		case MOD_MODE_CAPWAP:
		{

			struct mod_ac *cmod = modload_wtp("capwap");
			if (!cmod) {
				cw_log(LOG_ERR,
				       "Can't initialize mod_cisco, failed to load base mod mod_capwap");
				return 1;
			}
			cmod->register_actions(actions, MOD_MODE_CAPWAP);
			int rc = cipwap_register_actions_ac(actions);
			cw_dbg(DBG_INFO, "Initialized mod_cisco with %d actions", rc);
			return rc;
		}

/*		
		case MOD_MODE_BINDINGS:
		{
			struct mod_ac *cmod = modload_ac("capwap80211");
			if (!cmod) {
				cw_log(LOG_ERR,
				       "Can't initialize mod_cisco, failed to load base mod mod_capwap80211");
				return 1;
			}
			cmod->register_actions(actions, MOD_MODE_BINDINGS);
			int rc = cipwap_register_actions80211_ac(actions);
			cw_dbg(DBG_INFO, "Initialized mod_cisco 80211 with %d actions", rc);
			return 0;
		}
*/		
		

	}


	return 0;


}



static struct mod_ac cipwap_wtp = {
	.name ="cipwap",
	.init = cipwap_init,
	.detect = detect,
	.register_actions=register_actions

};



struct mod_ac * mod_cipwap_wtp(){
	return &cipwap_wtp;
};

