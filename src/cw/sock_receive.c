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

/**
 * @file
 * @brief Implementation of sock_receive
 */

#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "sock.h"

/**
 * Receive data from a socket
 * @param sock the socket
 * @param buf destination buffer
 * @param len length of destination buffer
 * @srcaddr receives the source address
 * @srcaddrlen receives the length of the source address
 * @return the number of bytes read oder -1 if an error has occured.
 *
 * If the return value is -1, errno is set to the appropriate error
 */
int sock_receive(int sock, void *buf, size_t len, int flags,
		 struct sockaddr *srcaddr, socklen_t * srcaddrlen)
{
//	socklen_t al = sizeof(struct sockaddr_storage);
	*srcaddrlen=sizeof(struct sockaddr);	
	memset(srcaddr, 0, sizeof(struct sockaddr));

	int n;
	while ((n = recvfrom(sock, (char *) buf, len, flags, srcaddr, srcaddrlen)) < 0) {
		if (errno != EINTR)
			return n;
	}
	return n;
}

