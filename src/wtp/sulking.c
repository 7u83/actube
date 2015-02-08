#include <unistd.h>

#include "capwap/cw_log.h"

#include "wtp.h"
#include "wtp_conf.h"

int sulking_state()
{
	cw_dbg(DBG_CW_INFO,"Entering Sulking state");
	cw_dbg(DBG_CW_INFO,"Sleeping for %i seconds",conf_silent_interval);
	sleep(conf_silent_interval);
	return 1;
}

