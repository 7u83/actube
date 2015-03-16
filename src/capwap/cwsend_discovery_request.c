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

/**
 * @file
 * @brief cwsend_discovery_request 
 */ 

#include "capwap.h"
#include "capwap_cisco.h"


#include "conn.h"
#include "cwmsg.h"

int cwsend_discovery_request(struct conn *conn, struct radioinfo *radioinfo,
			     struct wtpinfo *wtpinfo)
{
	uint8_t buffer[CWMSG_MAX_SIZE];
	struct cwmsg cwmsg;

	cwmsg_init(&cwmsg, buffer, CWMSG_DISCOVERY_REQUEST, conn_get_next_seqnum(conn),
		   NULL /*radioinfo */ );
	cwmsg.capwap_mode = conn->capwap_mode;


	/* Mandatory elements */

	/* discovery type */
	cwmsg_addelem(&cwmsg, CWMSGELEM_DISCOVERY_TYPE, &wtpinfo->discovery_type, sizeof(uint8_t));

	/* wtp board data */
	cwmsg_addelem_wtp_board_data(&cwmsg, wtpinfo);

	/* wtp descriptor */
	cwmsg_addelem_wtp_descriptor(&cwmsg, wtpinfo);

	/* wtp frame tunnel mode */
	cwmsg_addelem(&cwmsg, CWMSGELEM_WTP_FRAME_TUNNEL_MODE, &wtpinfo->frame_tunnel_mode,
		      sizeof(uint8_t));

	/* mac type */
	cwmsg_addelem(&cwmsg, CWMSGELEM_WTP_MAC_TYPE, &wtpinfo->mac_type, sizeof(uint8_t));


	/* radio infos */
	cwmsg_addelem_wtp_radio_infos(&cwmsg, wtpinfo->radioinfo);


	/* Non-mandatory elements */

	switch (cwmsg.capwap_mode) {
		case CWMODE_CISCO:
			cwmsg_addelem_vendor_cisco_rad_name(&cwmsg, (uint8_t *) wtpinfo->name);

			break;
		
		default:

			if (conn->mtu_discovery)
				cwmsg_addelem_mtu_discovery_padding(&cwmsg, conn);

	}

	return conn_send_cwmsg(conn, &cwmsg);
}
