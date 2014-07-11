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

#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "sock.h"

int sock_receive(int sock,void *buf, size_t len, int flags, struct sockaddr * srcaddr,socklen_t *addrlen)
{

	//unsigned int al;
	socklen_t al;

	al = sizeof (struct sockaddr_storage);
	
	memset(srcaddr, 0, sizeof(struct sockaddr));
//	srcaddr->sa_port=999;

	int n;
	while( (n = recvfrom(sock,(char*)buf,len,flags,srcaddr,&al)) < 0 ){
		if (errno!=EINTR)
			return n ;
	}
//	printf("Received betes %d\n",n);
//	char str[8000];
//	sock_addrtostr(srcaddr,str,1000);
//	printf("RECFROM: %s\n",str);

//	struct sockaddr_in * s = (struct sockaddr_in*)srcaddr;
//	s->sin_port=htons(78);
//	sock_addrtostr(srcaddr,str,1000);
//	printf("RECFROMset: %s\n",str);



//	exit(9);
	return n;
}
