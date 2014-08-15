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
#include "conn.h"
#include "cwmsg.h"

int cwsend_discovery_request(struct conn * conn,struct radioinfo * radioinfo,struct wtpinfo * wtpinfo)
{
	uint8_t buffer[CWMSG_MAX_SIZE];
	struct cwmsg cwmsg;

	cwmsg_init(&cwmsg,buffer,CWMSG_DISCOVERY_REQUEST,conn_get_next_seqnum(conn),radioinfo);
	
	cwmsg_addelem(&cwmsg,CWMSGELEM_DISCOVERY_TYPE,&wtpinfo->discovery_type,sizeof(uint8_t));
	cwmsg_addelem_wtp_board_data(&cwmsg,wtpinfo);
	cwmsg_addelem_wtp_descriptor(&cwmsg,wtpinfo);
	cwmsg_addelem(&cwmsg,CWMSGELEM_WTP_FRAME_TUNNEL_MODE,&wtpinfo->frame_tunnel_mode,sizeof(uint8_t));
	cwmsg_addelem(&cwmsg,CWMSGELEM_WTP_MAC_TYPE,&wtpinfo->mac_type,sizeof(uint8_t));
	cwmsg_addelem_wtp_radio_infos(&cwmsg,wtpinfo->radioinfo);
	cwmsg_addelem_mtu_discovery_padding(&cwmsg,conn);

	return conn_send_cwmsg(conn,&cwmsg);
}
