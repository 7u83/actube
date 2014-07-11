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

#include <errno.h>
#include <sys/socket.h>
#include "sock.h"
#include "conn.h"

#include <netinet/in.h>
#include <arpa/inet.h>


#include "cw_log.h"

int conn_send_packet(struct conn * conn, const uint8_t * buffer, int len)
{

#ifdef WITH_CW_LOG_DEBUG
	char addrstr[64];
	sock_addrtostr((struct sockaddr*)&conn->addr,addrstr,64);
	cw_log_debug1("Sending packet to %s, len=%d",addrstr,len);
	cw_log_debug2_dump(buffer,len,"Packet data for packet, sent to %s",addrstr);
#endif
	
	int n;
	while((n=sendto( conn->sock, buffer, len, 0, 
				(struct sockaddr*)&conn->addr, 
				sock_addrlen((struct sockaddr*)&conn->addr)))<0 ){

		if(errno == EINTR) 
			continue;
		return n;
        }

        return n;
}
