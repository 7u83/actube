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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "sock.h"


int sock_create(const struct sockaddr * sa)
{
	int sock;
	int yes=1;
	int salen;
	/* determine length */
	switch(sa->sa_family){
		case AF_INET:
			salen = sizeof(struct sockaddr_in);
			break;
		case AF_INET6:	
			salen = sizeof(struct sockaddr_in6);
			break;
		default:
			return -1;
	}


	/* bind a unicast address */
	if((sock = socket(sa->sa_family, SOCK_DGRAM, 0)) < 0) {
		return -1;
	}	

	/* reuse address */
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))){
		return -1;
	};

	/* bind address */
	if ( bind(sock,sa,salen) < 0) {
		return -1;
	}

	return sock;
}


