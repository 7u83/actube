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

void cwmsg_addelem_mtu_discovery_padding(struct cwmsg * msg, struct conn* conn)
{
	int len = conn->mtu - (msg->msgelems-msg->buffer+msg->pos)-4;

	if (len < 0 )
		return;
	
	uint32_t val = CWMSGELEM_MTU_DISCOVERY_PADDING<<16|len;
	*((uint32_t*)(msg->msgelems+msg->pos))=htonl(val);
	memset(msg->msgelems+4+msg->pos,0xff,len);	
	msg->pos+=4+len;
}

