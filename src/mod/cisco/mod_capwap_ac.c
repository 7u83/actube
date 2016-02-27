
#include "capwap/cw.h"

#include "capwap/mod.h"
#include "capwap/log.h"
#include "capwap/dbg.h"

#include "capwap/action.h"

#include "mod_capwap.h"


static struct cw_actiondef actions;



extern int capwap_register_actions_ac(struct cw_actiondef *def);


static int init()
{
	cw_dbg(DBG_INFO,"Initialiazing mod_capwap ...");
	int rc = capwap_register_actions_ac(&actions);
	cw_dbg(DBG_INFO,"Initialized mod capwap with %d actions",rc);
	return 0;
}


static int detect(struct conn *conn,const uint8_t *rawmsg, int rawlen,struct sockaddr *from)
{
	cw_foreach_elem(elem, elems_ptr, elems_len) {
	}


	cw_log(LOG_INFO,"Detecting ...");
	conn->detected=1;
	conn->actions=&actions;
	return 1;
}

static struct mod_ac capwap_ac = {
	.name ="capwap",
	.init = init,
	.detect = detect

};

struct mod_ac * mod_capwap_ac(){
	return &capwap_ac;
};

