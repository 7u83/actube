#include "capwap.h"
#include "capwap_ieee80211.h"

#include <stdio.h> //Tube
#include <stdlib.h>
#include <string.h>

#include "wtpinfo.h"
#include "cw_util.h"


//static int process_elem(struct wtpinfo *wtpinfo,int type,uint8_t* msgelem,int len)
static int process_elem(void *w,int type,uint8_t* msgelem,int len)
{
	struct wtpinfo * wtpinfo = (struct wtpinfo*)w;

	if (wtpinfo_readelem_location_data(wtpinfo,type,msgelem,len)) 
		return 1;

	if (wtpinfo_readelem_wtp_board_data(wtpinfo,type,msgelem,len))
		return 1;

	if (wtpinfo_readelem_wtp_descriptor(wtpinfo,type,msgelem,len)) 
		return 1;

	if (wtpinfo_readelem_wtp_name(wtpinfo,type,msgelem,len)) 
		return 1;

	if (wtpinfo_readelem_session_id(wtpinfo,type,msgelem,len)) 
		return 1;
	
	if (wtpinfo_readelem_wtp_frame_tunnel_mode(wtpinfo,type,msgelem,len)) 
		return 1;

	if (wtpinfo_readelem_wtp_mac_type(wtpinfo,type,msgelem,len)) 
		return 1;

	if (wtpinfo_readelem_wtp_radio_info(wtpinfo,type,msgelem,len)) 
		return 1;

	if (wtpinfo_readelem_ecn_support(wtpinfo,type,msgelem,len))
		return 1;

	if (wtpinfo_readelem_cw_local_ip_addr(wtpinfo,type,msgelem,len))
		return 1;

	return 0;
}


void process_join_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len)
{
	cw_foreach_msgelem(msg,len,process_elem,(void*)wtpinfo);
}

