
#include "wtpinfo.h"
#include "capwap.h"

#include "cw_log.h"


int wtpinfo_readelem_wtp_frame_tunnel_mode(struct wtpinfo * wtpinfo, int type, uint8_t * msgelem, int len)
{
	if (type != CWMSGELEM_WTP_FRAME_TUNNEL_MODE)
		return 0;

	if (len!=1){
		cw_dbg(DBG_CW_MSG_ERR,"Discarding WTP_DISCOVERY_TYPE msgelem, wrong size, type=%d,len=%d",type,len);
		return 1;
	}
	wtpinfo->frame_tunnel_mode=*msgelem;
	return 1;

}


