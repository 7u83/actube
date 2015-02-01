#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include "conn.h"


#include <stdio.h>

int conn_recv_packet(struct conn* conn,uint8_t *buf,int len)
{
	printf("THHHHHHHHHHHHHHHHHHHHHHHHHe  Conn recv packet called\n");
	int n;
	int flags=0;
	while( (n = recv(conn->sock,(char*)buf,len,flags)) < 0 ){
		if (errno!=EINTR)
		{
			if (errno == EWOULDBLOCK)
			{
				printf("would block\n");
				
//				continue;
			}
			if ( errno==EAGAIN )
			{
				printf("again\n");
			}
			perror("recv");
		printf("Return n %d\n",n);	
		return n;
		}
	}
	return n;
}


