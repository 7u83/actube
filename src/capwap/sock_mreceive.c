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

#include <stdint.h>
#include <sys/select.h>
#include <string.h>

#include "sock.h"



int sock_mreceive(int * socklist, int socklistlen, 
		void(*callback) (int * socklist, int index, struct sockaddr*,uint8_t*buf,int len))
{
	int n;
	fd_set fset;
	if ((n=sock_mwait(socklist,socklistlen,&fset))<0)
		return -1;


	int i;
	uint8_t buffer[SOCK_RECBUFFERSIZE];

	for( i=0; i<socklistlen; i++){
		if (socklist[i]<0)
			continue;

		if (!FD_ISSET(socklist[i],&fset))
			continue;

		struct sockaddr_storage srcaddr;
		socklen_t sockaddrlen;

		memset(&srcaddr,0,sizeof(struct sockaddr_storage));
		sockaddrlen = sizeof(struct sockaddr_storage);


		int len = sock_receive(socklist[i],
				buffer, SOCK_RECBUFFERSIZE,
				0,
				(struct sockaddr*)&srcaddr, &sockaddrlen);
		callback(socklist,i,(struct sockaddr*)&srcaddr,buffer,len);

	}
	return n;
}
