
#include "wtpinfo.h"
#include "capwap.h"

#include "cw_log.h"


int wtpinfo_readelem_discovery_type(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len)
{
	if (type != CWMSGELEM_DISCOVERY_TYPE)
		return 0;

	if (len!=1){
		cw_log_debug0("Discarding WTP_DISCOVERY_TYPE msgelem, wrong size, type=%d,len=%d",type,len);
		return 1;
	}

	wtpinfo->discovery_type=*msgelem;
	return 1;

}


