
#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/action.h"

#include "mod_capwap.h"


static struct cw_actiondef actions;



extern int capwap_register_actions_wtp(struct cw_actiondef *def);


static int init()
{
	cw_dbg(DBG_INFO, "Initialiazing mod_capwap ...");
	int rc = capwap_register_actions_wtp(&actions);
	cw_dbg(DBG_INFO, "Initialized mod capwap with %d actions", rc);
	return 0;
}


static int detect(struct conn *conn, const uint8_t * rawmsg, int rawlen, int elems_len,
		  struct sockaddr *from, int mode)
{
	if (mode != MOD_MODE_CAPWAP)
		return 0;
	conn->detected = 1;
	conn->actions = &actions;
	return 1;
}

static int register_actions(struct cw_actiondef *def, int mode)
{
	if (mode != MOD_MODE_CAPWAP)
		return 0;
	return capwap_register_actions_wtp(def);
}

static struct mod_wtp capwap_wtp = {
	.name = "capwap",
	.init = init,
	.detect = detect,
	.register_actions=register_actions
};

struct mod_wtp *mod_capwap_wtp()
{
	return &capwap_wtp;
};
