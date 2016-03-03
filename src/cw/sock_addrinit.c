#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void sock_addrinit(struct sockaddr *addr,int type)
{
	switch (type){
		case AF_INET:
			memset(addr,0,sizeof(struct sockaddr_in));
			addr->sa_family=type;
#ifdef HAVE_SIN_LEN
			addr_sa_len=sizeof(struct sockaddr_in);
#endif
			return;

	}
	

}
