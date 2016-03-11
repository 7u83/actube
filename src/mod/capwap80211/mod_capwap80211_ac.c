
#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/action.h"
#include "cw/cw.h"

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
	if (mode != MOD_MODE_BINDINGS)
		return 0;

	int wbid = cw_get_hdr_wbid(rawmsg);
	if (wbid != CW_WBID_IEEE80211 ) {
		cw_dbg(DBG_MOD,"CAPWAP80211 detected: no, my wbid=%d, remote wbid=%d",CW_WBID_IEEE80211,wbid);
		return 0;
	}
	cw_dbg(DBG_MOD,"CAPWAP80211 detected: yes");
	return 1;
}

static int register_actions(struct cw_actiondef *actions, int mode)
{
	if (mode != MOD_MODE_BINDINGS)
		return 0;

	return capwap80211_register_actions_ac(actions);
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
