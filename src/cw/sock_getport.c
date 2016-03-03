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

#include "sock.h"
#include <sys/socket.h>
#include <netinet/in.h>

int sock_getport(struct sockaddr *addr)
{
	switch (addr->sa_family){
		case AF_INET:
			return ntohs (((struct sockaddr_in*)addr)->sin_port); 
#ifdef WITH_IPV6
		case AF_INET6:
			return ntohs (((struct sockaddr_in6*)addr)->sin6_port); 
#endif
	}
	return -1;
}

