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
 *@brief Implementation of sock_cmpaddrs
 */

#include <stdint.h>
#include <string.h>
#include <netinet/in.h>

#include "sock.h"


/**
 * Compare two sockaddrs
 * @param addr1 first address to compare
 * @param addr2 second address to compare
 * @param cmpport if not zero, also compare the port
 * @return 0 if both addresses are equal, otherwise a value grater 
 * or less than zero, depending of the result
 *
 * Currently there are only AF_INET and AF_INET6 suppoorted.
 */

int sock_cmpaddr(const struct sockaddr *addr1, const struct sockaddr *addr2,int cmpport)
{

	uint8_t * s1, * s2;
	int p1,p2;
	int slen;

	if (!addr1){
		return -1;
	}

	if (!addr2){
		return -1;
	}

	if (addr1->sa_family != addr2->sa_family){
		return addr1->sa_family - addr2->sa_family;
	}

	switch (addr1->sa_family){
		case AF_INET:
			if (cmpport){
				p1 = ((struct sockaddr_in*)addr1)->sin_port;
				p2 = ((struct sockaddr_in*)addr2)->sin_port;
				if (p1!=p2)
					return p1-p2;
			}
			s1 = (uint8_t*)&(((struct sockaddr_in*)addr1)->sin_addr);
			s2 = (uint8_t*)&(((struct sockaddr_in*)addr2)->sin_addr);
			slen = sizeof(struct in_addr);
			break;
		case AF_INET6:
			if (cmpport){
				p1 = ((struct sockaddr_in6*)addr1)->sin6_port;
				p2 = ((struct sockaddr_in6*)addr2)->sin6_port;
				if (p1!=p2)
					return p1-p2;
			}
			s1 = (uint8_t*)&(((struct sockaddr_in6*)addr1)->sin6_addr);
			s2 = (uint8_t*)&(((struct sockaddr_in6*)addr2)->sin6_addr);
			slen = sizeof(struct in6_addr);
			break;
		default:

			return -1;	
	}
	return memcmp(s1,s2,slen);
}
