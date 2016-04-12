#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "sock.h"

int sock_set_recvtimeout(int sock, int seconds)
{
	struct timeval timeval;
	memset(&timeval,0,sizeof(timeval));
	timeval.tv_sec=seconds;
	return setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&timeval,sizeof(timeval));
}
