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



#include <string.h>

#include "capwap.h"
#include "capwap_cisco.h"

#include "conn.h"
#include "cwmsg.h"
#include "sock.h"

#include "cw_log.h"

void cwsend_discovery_response(struct conn *conn, int seqnum, struct radioinfo *radioinfo,
			       struct ac_info *acinfo, struct wtpinfo *wtpinfo)
{
	cw_dbg(DBG_CW_MSG, "Sending discovery response to %s, seq = %d", sock_addr2str(&conn->addr),
	       seqnum);

	struct cwmsg *cwmsg = &conn->resp_msg;
	cwmsg_init(cwmsg, conn->resp_buffer, CWMSG_DISCOVERY_RESPONSE, seqnum, NULL);
	cwmsg->capwap_mode = conn->capwap_mode;


	cwmsg_addelem_ac_descriptor(cwmsg, acinfo,wtpinfo);
	cwmsg_addelem(cwmsg, CW_ELEM_AC_NAME, (uint8_t *) acinfo->ac_name,
		      strlen((char *) acinfo->ac_name));

	cwmsg_addelem_wtp_radio_infos(cwmsg, acinfo->radioinfos);
	cwmsg_addelem_ctrl_ip_addrs(cwmsg, acinfo);


	/* Send Cisco-specific message elements if needed  */
	switch (cwmsg->capwap_mode) {
		case CWMODE_CISCO:
		case CWMODE_CIPWAP:
		{
			cwmsg_addelem_vendor_cisco_ap_timesync(cwmsg);
			uint8_t mwtype=1;
			cwmsg_addelem_vendor_s_payload(cwmsg,CW_VENDOR_ID_CISCO,CW_CISCO_MWAR_TYPE,&mwtype,1);
			break;
		}
		default:
			break;

	}



	conn_send_response(conn, cwmsg, seqnum);
}
