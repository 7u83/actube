#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include "conn.h"


int conn_recv_packet(struct conn* conn,uint8_t *buf,int len)
{
//	printf("Conn recv packet called\n");
	int n;
	int flags=0;
	while( (n = recv(conn->sock,(char*)buf,len,flags)) < 0 ){
		if (errno!=EINTR)
		{
/*			if (errno == EWOULDBLOCK)
			{
				printf("would block\n");
			}
			if ( errno==EAGAIN )
			{
				printf("again\n");
			}
			perror("recv");
*/			
			return n;
		}
	}
	return n;
}


