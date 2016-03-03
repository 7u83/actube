#include "sock.h"

#include <sys/socket.h>
#include <netinet/in.h>

int sock_setport(struct sockaddr *addr, int port)
{
	switch (addr->sa_family){
		case AF_INET:
			((struct sockaddr_in*)addr)->sin_port=htons(port);
			break;
#ifdef WITH_IPV6
		case AF_INET6:
			((struct sockaddr_in6*)addr)->sin6_port=htonl(port);
			break;
#endif
		default:
			return 0;
	}
	return 1;
}
