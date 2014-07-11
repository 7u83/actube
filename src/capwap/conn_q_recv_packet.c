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
#include <errno.h>

#include "conn.h"

int conn_q_recv_packet(struct conn * conn, uint8_t * buffer,int len)
{
	if ( !conn->cur_packet)
	{
		if ((conn->cur_packet = conn_q_get_packet(conn)) == 0){
			errno = EAGAIN;
			return -1;
		}
		conn->cur_packet_len = *((uint32_t*)conn->cur_packet);
		conn->cur_packet_pos=4;
	}

	if (conn->cur_packet_len > len )
	{
		memcpy(buffer,conn->cur_packet+conn->cur_packet_pos,len);
		conn->cur_packet_pos+=len;
		conn->cur_packet_len-=len;
		return len;

	}

	memcpy(buffer,conn->cur_packet+conn->cur_packet_pos,conn->cur_packet_len);
	free (conn->cur_packet);
	conn->cur_packet=0;
	return conn->cur_packet_len;	
}
