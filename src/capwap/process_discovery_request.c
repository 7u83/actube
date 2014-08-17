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

#include <stdlib.h>
#include <string.h>

#include "capwap.h"
#include "capwap_ieee80211.h"

#include "cw_log.h"
#include "cw_util.h"

static int wtpinfo_readelem_discovery_req(void * w,int type,uint8_t* msgelem,int len)
{
	struct wtpinfo * wtpinfo = (struct wtpinfo *)w;
	cw_dbg(DBG_CW_MSGELEM,"Process discovery req msgelem, type=%d (%s), len=%d",type,cw_msgelemtostr(type),len);
	cw_dbg_dmp(DBG_CW_MSGELEM_DMP,msgelem,len,"Dump for msgelem ...");

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

