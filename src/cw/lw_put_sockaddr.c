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


#include <string.h>
#include <netinet/in.h>

#include "lw.h"

int lw_put_sockaddr(uint8_t * dst, struct sockaddr_storage *addr)
{
	switch (addr->ss_family) {
		case AF_INET:
			{
				struct sockaddr_in *sain = (struct sockaddr_in *) addr;
				memcpy(dst, &sain->sin_addr.s_addr,
				       sizeof(sain->sin_addr.s_addr));
				return sizeof(sain->sin_addr.s_addr);
			}
		case AF_INET6:
			{
				struct sockaddr_in6 *sain = (struct sockaddr_in6 *) addr;
				memcpy(dst, &sain->sin6_addr.s6_addr,
				       sizeof(sain->sin6_addr.s6_addr));
				return sizeof(sain->sin6_addr.s6_addr);
			}
	}

	return 0;
}
