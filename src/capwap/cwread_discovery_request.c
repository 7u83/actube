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
#include "capwap_80211.h"

#include "cw_log.h"
#include "cw_util.h"


struct eparm {
	int *mand;
	struct wtpinfo *wtpinfo;
};

static int wtpinfo_readelem_discovery_req(void *eparm, int type,
					  uint8_t * msgelem, int len)
{
	struct eparm *e = (struct eparm *) eparm;
	cw_dbg_msgelem(CWMSG_DISCOVERY_REQUEST, type, msgelem, len);

	/* mandatory message elements */
	if (wtpinfo_readelem_discovery_type
	    (e->wtpinfo, type, msgelem, len))
		goto foundX;

	if (wtpinfo_readelem_wtp_board_data
	    (e->wtpinfo, type, msgelem, len))
		goto foundX;

	if (wtpinfo_readelem_wtp_descriptor
	    (e->wtpinfo, type, msgelem, len))
		goto foundX;

	if (wtpinfo_readelem_wtp_frame_tunnel_mode
	    (e->wtpinfo, type, msgelem, len))
		goto foundX;

	if (wtpinfo_readelem_wtp_mac_type(e->wtpinfo, type, msgelem, len))
		goto foundX;

	if (wtpinfo_readelem_wtp_radio_info
	    (e->wtpinfo, type, msgelem, len))
		goto foundX;

	/* non-mandatory mesage lements */
	if (cw_readelem_mtu_discovery_padding(type, msgelem, len))
		return 1;

	if (cw_readelem_vendor_specific_payload
	    (e->wtpinfo, CWMSG_DISCOVERY_REQUEST, type, msgelem, len))
		return 1;

	return 0;
      foundX:
	cw_mand_elem_found(e->mand, type);
	return 1;
}


void cwread_discovery_request(struct wtpinfo *wtpinfo, uint8_t * msg,
			      int len)
{
	int mand[] = {
		CWMSGELEM_DISCOVERY_TYPE,
		CWMSGELEM_WTP_BOARD_DATA,
		CWMSGELEM_WTP_DESCRIPTOR,
		CWMSGELEM_WTP_FRAME_TUNNEL_MODE,
		CWMSGELEM_WTP_MAC_TYPE,
		CWMSGELEM_80211_WTP_RADIO_INFO,
		-1
	};

	struct eparm eparm;
	eparm.wtpinfo = wtpinfo;
	eparm.mand = mand;

	cw_foreach_msgelem(msg, len, wtpinfo_readelem_discovery_req,
			   &eparm);

	cw_dbg_missing_mand_elems_(0, CWMSG_DISCOVERY_REQUEST, eparm.mand);
}
