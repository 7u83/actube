#include <unistd.h>

#include "capwap/cw_log.h"

#include "wtp.h"
#include "wtp_conf.h"

int sulking_state()
{
	cw_log_debug0("Entering Sulking state");
	cw_log_debug0("Sleeping for %i seconds",conf_silent_interval);
	sleep(conf_silent_interval);
	return 1;
}

