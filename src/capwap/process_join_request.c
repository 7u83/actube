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

struct eparm {
	int *mand;
	struct wtpinfo *wtpinfo;
};


static int process_elem(void *eparm,int type,uint8_t* msgelem,int len)
{

	struct eparm *e = (struct eparm *) eparm;

	struct wtpinfo * wtpinfo = e->wtpinfo;


	cw_dbg_msgelem(CWMSG_JOIN_REQUEST, type, msgelem, len);


	/* mandatory elements */
	if (wtpinfo_readelem_location_data(wtpinfo,type,msgelem,len)) 
		goto foundX;

	if (wtpinfo_readelem_wtp_board_data(wtpinfo,type,msgelem,len))
		goto foundX;

	if (wtpinfo_readelem_wtp_descriptor(wtpinfo,type,msgelem,len)) 
		goto foundX;

	if (wtpinfo_readelem_wtp_name(wtpinfo,type,msgelem,len)) 
		goto foundX;

	if (wtpinfo_readelem_session_id(wtpinfo,type,msgelem,len)) 
		goto foundX;
	
	if (wtpinfo_readelem_wtp_frame_tunnel_mode(wtpinfo,type,msgelem,len)) 
		goto foundX;

	if (wtpinfo_readelem_wtp_mac_type(wtpinfo,type,msgelem,len)) 
		goto foundX;

	if (wtpinfo_readelem_ecn_support(wtpinfo,type,msgelem,len))
		goto foundX;

	if (cw_readelem_capwap_local_ip_addr(wtpinfo,type,msgelem,len)){
		cw_mand_elem_found(e->mand, XCWMSGELEM_CAPWAP_LOCAL_IP_ADDRESS);
		return 1;
	}

	/* understood capwap bindings */
	if (wtpinfo_readelem_wtp_radio_info(wtpinfo,type,msgelem,len)){
		cw_mand_elem_found(e->mand, XCWMSGELEM_CAPWAP_RADIO_INFO);
		return 1;
	}

	/* non-mandatory messages */
	
	uint16_t ml;
	if (cw_readelem_maximum_message_length(&ml,type,msgelem,len)){
		return 1;
	}

	if (cw_readelem_wtp_reboot_statistics(&e->wtpinfo->reboot_statistics,type,msgelem,len))
		goto foundX;

	return 0;

      foundX:
	cw_mand_elem_found(e->mand, type);
	return 1;
}


void process_join_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len)
{
	int mand[] = {
		CWMSGELEM_LOCATION_DATA,
		CWMSGELEM_WTP_BOARD_DATA,
		CWMSGELEM_WTP_DESCRIPTOR,
		CWMSGELEM_WTP_NAME,
		CWMSGELEM_WTP_FRAME_TUNNEL_MODE,
		CWMSGELEM_WTP_MAC_TYPE,
		CWMSGELEM_ECN_SUPPORT,
		XCWMSGELEM_CAPWAP_LOCAL_IP_ADDRESS,
		XCWMSGELEM_CAPWAP_RADIO_INFO,
		-1
	};


	struct eparm eparm;
	eparm.wtpinfo = wtpinfo;
	eparm.mand = mand;

	cw_foreach_msgelem(msg, len, process_elem,
			   &eparm);

	cw_dbg_missing_mand_elems_(0, CWMSG_DISCOVERY_REQUEST, eparm.mand);

}

