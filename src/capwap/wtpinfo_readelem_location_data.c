
#include "wtpinfo.h"
#include "capwap.h"

#include "cw_util.h"
#include "cw_log.h"


int wtpinfo_readelem_location_data(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len)
{
	if (type != CWMSGELEM_LOCATION_DATA)
		return 0;

	cw_setstr(&wtpinfo->location,msgelem,len);
	return 1;

}


