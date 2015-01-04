/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "capwap.h"
#include "capwap_ieee80211.h"

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "wtpinfo.h"

#include "cw_util.h"
#include "cw_log.h"

static int process_elem(void *w,int type,uint8_t* msgelem,int len)
{
	struct wtpinfo * wtpinfo = (struct wtpinfo*)w;


	cw_dbg_msgelem(CWMSG_JOIN_REQUEST, type, msgelem, len);

//	cw_dbg(DBG_CW_MSGELEM,"Process join req msgelem, type=%d (%s), len=%d",type,cw_msgelemtostr(type),len);
//	cw_dbg_dmp(DBG_CW_MSGELEM_DMP,msgelem,len,"Dump for msgelem ...");


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

