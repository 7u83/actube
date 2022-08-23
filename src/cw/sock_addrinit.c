#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>

#include "sock.h"

void sock_addrinit(struct sockaddr_storage *addr,int type)
{
	int len;
	switch (type){
		case AF_INET:
			len=sizeof(struct sockaddr_in);
			break;
		case AF_INET6:
			len=sizeof(struct sockaddr_in6);
			break;
		case AF_UNIX:
			len = sizeof(struct sockaddr_un);
			break;
		default:
			len=0;
	}
	
	memset(addr,0,len);
	addr->ss_family=type;
#ifdef HAVE_SS_LEN
	addr->ss_len=len;
#endif	

}
