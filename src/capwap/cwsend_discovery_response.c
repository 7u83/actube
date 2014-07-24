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
#include "conn.h"
#include "cwmsg.h"

void cwsend_discovery_response(struct conn * conn,int seqnum, struct radioinfo * radioinfo,  struct ac_info * acinfo, struct wtpinfo * wtpinfo)
{
	struct cwmsg * cwmsg = &conn->swm;	
	cwmsg_init(cwmsg,conn->buffer,CWMSG_DISCOVERY_RESPONSE,seqnum,radioinfo);
	
	cwmsg_addelem_ac_descriptor(cwmsg,acinfo);
	cwmsg_addelem(cwmsg,CWMSGELEM_AC_NAME,(uint8_t*)acinfo->ac_name,strlen((char*)acinfo->ac_name));
	cwmsg_addelem_wtp_radio_infos(cwmsg,wtpinfo);
	cwmsg_addelem_ctrl_ip_addrs(cwmsg,acinfo);

	
	conn_send_response(conn,cwmsg,seqnum);
}

