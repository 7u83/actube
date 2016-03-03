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
    along with libcapwap.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 *@file
 *@brief Implementation of sock_getbroadcastaddr
 */


#include <string.h>
#include <ifaddrs.h>

#include "sock.h"

/**
 * Get the broadcast adddress
 * @param addr address to search
 * @param broadcastaddr receives the resulting broadcast address
 * @return 1 if successful, 0 if no braodcast address was determined
 */
int sock_getifinfo(const struct sockaddr *addr, char *ifname, struct sockaddr *broadcast,
		   struct sockaddr *netmask)
{
	struct ifaddrs *ifap, *ifa;

	if (getifaddrs(&ifap) == -1)
		return 0;

	int rc = 0;
	for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {

		if (sock_cmpaddr(addr, ifa->ifa_addr, 0))
			continue;

		if (addr->sa_family == AF_INET ) {

			if (broadcast)
				memcpy(broadcast, ifa->ifa_broadaddr,
				       sock_addrlen(ifa->ifa_broadaddr));
			if (netmask)
				memcpy(netmask, ifa->ifa_netmask,
				       sock_addrlen(ifa->ifa_netmask));
		}
		if (ifname)
			strcpy(ifname, ifa->ifa_name);	//, strlen(ifa->ifa_name));

		rc = 1;
		break;
	}
	freeifaddrs(ifap);
	return rc;
}
