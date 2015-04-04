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
 *@file
 *@brief Implementation of sock_addrlen.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifdef AF_LINK
#   include <net/if_dl.h>
#endif
#ifdef AF_PACKET
#   include <netpacket/packet.h>
#endif

#include "sock.h"

/**
 * Determine the length of a sockaddr struct
 * @param sa pointer to socket address
 * @return length of socket
 */
int sock_addrlen(const struct sockaddr * sa)
{

	/* determine length */
	switch(sa->sa_family){
		case AF_INET:
			return sizeof(struct sockaddr_in);
		case AF_INET6:	
			return  sizeof(struct sockaddr_in6);

#ifdef AF_PACKET
		case AF_PACKET:
			return sizeof(struct sockaddr_ll);	
#endif 

#ifdef AF_LINK
		case AF_LINK:
			return sizeof(struct sockaddr_dl);	
#endif 
		default:
			return 0;
	}

}

