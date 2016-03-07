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

#include "sock.h"

#include <sys/socket.h>
#include <netinet/in.h>

/**
 * @file 
 * @brief implements sock_setport
 */

/**
 * @addtogroup SOCK
 * @{
 */

/**
 * Set the port number of a sockaddr
 * @param addr sockaddr where the port will be set
 * @param port number
 * @return 1 if successful, otherwise 0
 *
 * Ports can only be set for sockaddrs of type AF_INET and AF_INET6
 */
int sock_setport(struct sockaddr *addr, int port)
{
	switch (addr->sa_family){
		case AF_INET:
			((struct sockaddr_in*)addr)->sin_port=htons(port);
			break;
		case AF_INET6:
			((struct sockaddr_in6*)addr)->sin6_port=htons(port);
			break;
		default:
			return 0;
	}
	return 1;
}

/**
 * @}
 */

