
#include "capwap/mod.h"
#include "capwap/log.h"
#include "capwap/dbg.h"

#include "capwap/action.h"

#include "mod_cisco.h"


static struct cw_actiondef actions;


#include "../modload.h"

extern int cisco_register_actions_ac(struct cw_actiondef *def);


static int init()
{
	struct mod_ac * cmod = modload_ac("capwap");

	cmod->register_actions(&actions);



	cw_dbg(DBG_INFO,"Initialiazing mod_cisco ...");
	int rc = cisco_register_actions_ac(&actions);
	cw_dbg(DBG_INFO,"Initialized mod cisco with %d actions",rc);
	return 0;
}


static int detect(struct conn *conn,const uint8_t *rawmsg, int rawlen,struct sockaddr *from)
{
	cw_log(LOG_INFO,"Detecting ...");
	conn->detected=1;
	conn->actions=&actions;
	return 1;
}

static struct mod_ac capwap_ac = {
	.name ="cisco",
	.init = init,
	.detect = detect

};

struct mod_ac * mod_cisco_ac(){
	return &capwap_ac;
};

