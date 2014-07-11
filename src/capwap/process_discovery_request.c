
#include <stdio.h> //tube
#include <stdlib.h>
#include <string.h>

#include "capwap.h"
#include "capwap_ieee80211.h"

#include "cw_log.h"
#include "cw_util.h"

//static int wtpinfo_readelem_discovery_req(struct wtpinfo *wtpinfo,int type,uint8_t* msgelem,int len)
static int wtpinfo_readelem_discovery_req(void * w,int type,uint8_t* msgelem,int len)
{
	struct wtpinfo * wtpinfo = (struct wtpinfo *)w;
	cw_log_debug1("Process discovery req msgelem, type=%d, len=%d\n",type,len);

	if (wtpinfo_readelem_discovery_type(wtpinfo,type,msgelem,len)) 
		return 1;

	if (wtpinfo_readelem_wtp_board_data(wtpinfo,type,msgelem,len))
		return 1;

	if (wtpinfo_readelem_wtp_descriptor(wtpinfo,type,msgelem,len)) 
		return 1;

	if (wtpinfo_readelem_wtp_frame_tunnel_mode(wtpinfo,type,msgelem,len)) 
		return 1;

	if (wtpinfo_readelem_wtp_mac_type(wtpinfo,type,msgelem,len)) 
		return 1;

	if (wtpinfo_readelem_wtp_radio_info(wtpinfo,type,msgelem,len))
		return 1;

	return 0;

}



void process_discovery_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len)
{
	cw_foreach_msgelem(msg,len,wtpinfo_readelem_discovery_req,wtpinfo);
}



