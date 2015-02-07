
#include "wtpinfo.h"
#include "capwap.h"

#include "cw_log.h"

int wtpinfo_readelem_wtp_mac_type(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len)
{
	if (type != CWMSGELEM_WTP_MAC_TYPE)
		return 0;

	if (len!=1){
		cw_dbg(DBG_CW_MSG_ERR,"Discarding WTP_MAC_TYPE msgelem, wrong size, type=%d,len=%d",type,len);
		return 1;
	}

	wtpinfo->mac_type=*msgelem;
	return 1;
}


