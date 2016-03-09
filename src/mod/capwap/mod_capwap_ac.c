
#include "cw/mod.h"
#include "cw/log.h"
#include "cw/dbg.h"

#include "cw/action.h"

#include "mod_capwap.h"


static struct cw_actiondef actions;



extern int capwap_register_actions_ac(struct cw_actiondef *def);


static int init()
{
	cw_dbg(DBG_MOD, "Initialiazing mod_capwap.");
//	int rc = capwap_register_actions_ac(&actions);
//	cw_dbg(DBG_INFO, "Initialized mod capwap with %d actions", rc);
	return 0;
}


static int detect(struct conn *conn, const uint8_t * rawmsg, int rawlen, int elems_len,
		  struct sockaddr *from, int mode)
{
	if (mode != MOD_DETECT_CAPWAP)
		return 0;
	cw_dbg(DBG_MOD,"CAPWAP detected: yes");
	return 1;
}

static struct mod_ac capwap_ac = {
	.name = "capwap",
	.init = init,
	.detect = detect,
	.register_actions = capwap_register_actions_ac
};

struct mod_ac *mod_capwap_ac()
{
	return &capwap_ac;
};
