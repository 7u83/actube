#include <unistd.h>

#include "cw/dbg.h"

#include "wtp.h"
#include "wtp_conf.h"

int sulking_state()
{
	cw_dbg(DBG_INFO,"Sulking state, sleeping for %i seconds",conf_silent_interval);
	sleep(conf_silent_interval);
	return 1;
}

