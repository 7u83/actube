
#include "capwap/mod.h"
#include "capwap/log.h"

#include "mod_cipwap.h"



int cipwap_init()
{
	return 0;
}


static int detect(struct conn *conn,const uint8_t *rawmsg, int rawlen,int elems_len, struct sockaddr *from)
{
	cw_log(LOG_INFO,"Detecting ...");
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

