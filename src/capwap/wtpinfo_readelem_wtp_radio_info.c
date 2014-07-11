#include "wtpinfo.h"
#include "capwap_ieee80211.h"

#include "cw_log.h"


int wtpinfo_readelem_wtp_radio_info(struct wtpinfo * wtpinfo,int type,uint8_t *msgelem, int len)
{
	if (type != CWMSGELEM_IEEE80211_WTP_RADIO_INFO)
		return 0;

	if (len!=5){
		cw_log_debug0("Discarding msgelem WTP_RADIO_INFO, wrong size, len=%d\n",len);
		return -1;
	}

	int rid=*msgelem & 0x1f;
	wtpinfo->radioinfo[rid].rid=rid;
	wtpinfo->radioinfo[rid].type=*(uint32_t*)(msgelem+4);
	return 1;
}

