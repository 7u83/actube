/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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
#include "capwap.h"

#include "conn.h"

#include <netinet/in.h>
#include <arpa/inet.h>

#include "dbg.h"


int conn_send_data_packet(struct cw_Conn * conn, const uint8_t * buffer, int len)
{
	char sock_buf[SOCK_ADDR_BUFSIZE];
	int n;

	cw_dbg(DBG_X,"Sending data packet to %s\n",sock_addr2str_p(&conn->data_addr,sock_buf));

	while((n=sendto( conn->data_sock, buffer, len, 0, 
				(struct sockaddr*)&conn->data_addr, 
				sock_addrlen((struct sockaddr*)&conn->data_addr)))<0 ){

		if(errno == EINTR) 
			continue;


		return n;
        }

        return n;
}
