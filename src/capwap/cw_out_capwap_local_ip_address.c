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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include "capwap.h"
#include "capwap_items.h"

#include "dbg.h"
#include "log.h"
#include "capwap.h"
#include "conn.h"

int cw_out_capwap_local_ip_address(struct conn *conn, struct cw_action_out *action, uint8_t * dst)
{
        struct sockaddr_storage a;
        socklen_t alen = sizeof(struct sockaddr_storage);
	getsockname (conn->sock,(struct sockaddr *)&a,&alen);


	switch (((struct sockaddr*)&a)->sa_family){
		case AF_INET:
		{
			struct sockaddr_in  * sain = (struct sockaddr_in*)&a;
			int id;
			if (conn->capwap_mode == CW_MODE_CISCO)
				id = CW_ELEM_WTP_IPV4_IP_ADDRESS; 
			else
				id = CW_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS;
			
			cw_put_data(dst+4,(uint8_t*)&sain->sin_addr,4);
			return 4+cw_put_elem_hdr(dst,id,4);
		}

		case AF_INET6:
		{
			int id;
			if (conn->capwap_mode == CW_MODE_CISCO)
				id = CW_ELEM_WTP_IPV6_IP_ADDRESS; 
			else
				id = CW_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS;
			struct sockaddr_in6  * sain = (struct sockaddr_in6*)&a;
			cw_put_data(dst,(uint8_t*)&sain->sin6_addr,16);
			return 16+cw_put_elem_hdr(dst,id,16);

		}
	}


	return 0;
}


