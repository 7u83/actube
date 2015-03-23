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
#include "cwmsg.h"
#include "conn.h"

#include "sock.h" //Tube

void cwmsg_addelem_cw_local_ip_addr(struct cwmsg *msg, struct conn * conn)
{

        struct sockaddr_storage a;
        socklen_t alen = sizeof(struct sockaddr_storage);
	getsockname (conn->sock,(struct sockaddr *)&a,&alen);

	int cw_mode = msg->capwap_mode;
cw_mode = CWMODE_CISCO;


	switch (((struct sockaddr*)&a)->sa_family){
		case AF_INET:
		{
			struct sockaddr_in  * sain = (struct sockaddr_in*)&a;
			int id;
			if (cw_mode == CWMODE_CISCO)
				id = CW_ELEM_WTP_IPV4_IP_ADDRESS; 
			else
				id = CWMSGELEM_CAPWAP_LOCAL_IPV4_ADDRESS;

printf("Sending local ip %s\n",sock_addr2str(sain));
			
			cwmsg_addelem(msg,id,(uint8_t*)&sain->sin_addr,4);
			break;
		}


		case AF_INET6:
		{
			int id;
			if (cw_mode == CWMODE_CISCO)
				id = CW_ELEM_WTP_IPV6_IP_ADDRESS; 
			else
				id = CWMSGELEM_CAPWAP_LOCAL_IPV6_ADDRESS;
			struct sockaddr_in6  * sain = (struct sockaddr_in6*)&a;
			return cwmsg_addelem(msg,id,(uint8_t*)&sain->sin6_addr,16);
		}
		break;
	}

}


