
#include "wtpinfo.h"
#include "capwap.h"

#include "cw_util.h"
#include "cw_log.h"


int wtpinfo_readelem_wtp_name(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len)
{
	if (type != CWMSGELEM_WTP_NAME)
		return 0;

	if (len>512){
		cw_dbg(DBG_CW_MSG_ERR,"Truncating WTP_NAME msgelem to 512, wrong size, type=%d,len=%d",type,len);
		len=512;
	}

	cw_setstr(&wtpinfo->name,msgelem,len);
	return 1;
}


