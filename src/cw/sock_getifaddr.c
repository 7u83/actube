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

#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>

#include "sock.h"

int sock_getifaddr(const char * ifname,int family, int type,struct sockaddr * sa)
{
        struct ifaddrs *ifap,*ifa;
        
	if ( getifaddrs(&ifap)==-1)
		return 0;

	int rc=0;
        for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {
		if (strcmp(ifname,ifa->ifa_name))
			continue;

		if (ifa->ifa_addr == NULL)
			continue;

		if (ifa->ifa_addr->sa_family != family)
			continue;

		if (type != 0)
			if (!(ifa->ifa_flags & type))
				continue;

		switch (type){
			case 0:
				memcpy (sa, ifa->ifa_addr, sock_addrlen(ifa->ifa_addr));
				rc=1;
				break;
			case IFF_BROADCAST:
//				memcpy (sa, ifa->ifa_ifu.ifu_broadaddr, sock_addrlen(ifa->ifa_addr));
				memcpy (sa, ifa->ifa_broadaddr, sock_addrlen(ifa->ifa_addr));
				rc=1;
				break;
			default:
				break;

		}
//		rc=1;
		break;


        }

//	printf("safam: %i\n",sa->sa_family);
        freeifaddrs(ifap);
	return rc;
}


