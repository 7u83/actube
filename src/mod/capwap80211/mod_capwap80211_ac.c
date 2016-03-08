
#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/action.h"

#include "mod_capwap80211.h"





//extern int capwap_register_actions_ac(struct cw_actiondef *def);


static int init()
{
	cw_dbg(DBG_INFO, "Initialiazing mod_capwap80211 ...");
//	int rc = capwap_register_actions_ac(&actions);
	//cw_dbg(DBG_INFO, "Initialized mod capwap80211 with %d actions", rc);
	return 0;
}


static int detect(struct conn *conn, const uint8_t * rawmsg, int rawlen, int elems_len,
		  struct sockaddr *from, int mode)
{
	if (mode != MOD_DETECT_BINDINGS)
		return 0;

	cw_log(LOG_INFO, "Detecting 80211");
	return 1;
}

static int register_actions(struct cw_actiondef *actions)
{

	return 0;
}

static struct mod_ac capwap80211_ac = {
	.name = "capwap80211",
	.init = init,
	.detect = detect,
	.register_actions = register_actions
};

struct mod_ac *mod_capwap80211_ac()
{
	return &capwap80211_ac;
};
