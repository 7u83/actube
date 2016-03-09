
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
	if (mode != MOD_DETECT_CAPWAP)
		return 0;

	cw_dbg(DBG_MOD,"CIPWAP detected: no");
	return 0;
}



static struct mod_ac cipwap_ac = {
	.name ="cipwap",
	.init = cipwap_init,
	.detect = detect

};



struct mod_ac * mod_cipwap_ac(){
	return &cipwap_ac;
};

