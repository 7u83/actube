#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#include "sock.h"



int sock_getbroadcastaddr(const struct sockaddr * addr,struct sockaddr * broadcastaddr)
{
        struct ifaddrs *ifap,*ifa;
        
	if ( getifaddrs(&ifap)==-1)
		return 0;

	int rc=0;
        for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next) {

		if (sock_cmpaddr(addr,ifa->ifa_addr,0))
			continue;
		memcpy (broadcastaddr, ifa->ifa_broadaddr, sock_addrlen(ifa->ifa_broadaddr));
		rc=1;
		break;
        }
        freeifaddrs(ifap);
	return rc;
}


