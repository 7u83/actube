
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "sock.h"

void sock_copyaddr(struct sockaddr_storage *dst, const struct sockaddr * src){
	int salen;
	/* determine length */
	switch(src->sa_family){
		case AF_INET:
			salen = sizeof(struct sockaddr_in);
			break;
#ifdef WITH_IPV6
		case AF_INET6:	
			salen = sizeof(struct sockaddr_in6);
			break;
#endif
		default:
			return;
	}

	memcpy(dst,src,salen);

}

