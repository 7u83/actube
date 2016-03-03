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

#include <stdlib.h>
#include <string.h>

#include <net/if.h>
#include <ifaddrs.h>

#include "sock.h"


char * sock_get_primary_if(int family)
{
        struct ifaddrs *ifap,*ifa;
	char * r = 0;

        getifaddrs(&ifap);

        for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_flags & IFF_LOOPBACK){
			continue;
		}

		if (ifa->ifa_addr == NULL)
			continue;

		if (ifa->ifa_addr->sa_family != family)
			continue;

		r = malloc(strlen(ifa->ifa_name)+1);
		if (r)
			strcpy(r,ifa->ifa_name);
		break;

        }
        freeifaddrs(ifap);
	return r;
}


