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
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#ifdef AF_LINK
#   include <net/if_dl.h>
#endif
#ifdef AF_PACKET
#   include <netpacket/packet.h>
#endif

#include "sock.h"

/*
void sock_hwaddrtostr(const uint8_t *haddr,int len,char *dst)
{
	int i;
	for (i=0; i<len-1; i++){
		sprintf(dst,"%02X:",haddr[i]);
		dst+=3;
	}
	sprintf(dst,"%02X",haddr[i]);

}
*/

char *sock_addrtostr(const struct sockaddr *sa, char *s, size_t maxlen)
{

	int port;

	switch(sa->sa_family) {
		case AF_INET:
			inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), s, maxlen);
			port = ((struct sockaddr_in *)sa)->sin_port;
			sprintf(s,"%s:%i",s,ntohs(port));
			break;

	        case AF_INET6:
			inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), s, maxlen);
			break;
#ifdef AF_LINK
		case AF_LINK:
			{
				struct sockaddr_dl *sl=(struct sockaddr_dl*)sa;
				sock_hwaddrtostr(((uint8_t*)sl->sdl_data)+sl->sdl_nlen,sl->sdl_alen,s);

			}
			break;

#endif	/* AF_LINLK */

#ifdef AF_PACKET
		case AF_PACKET:
			{
				int i;
				char *sp=s;
				struct sockaddr_ll *sl=(struct sockaddr_ll*)sa;
				for (i=0; i<sl->sll_halen-1; i++){
					sprintf(sp,"%02X:",sl->sll_addr[i]);
					sp+=3;
				}
				sprintf(sp,"%02X",sl->sll_addr[i]);
			}
			break;
#endif /* AF_PACKET */


        	default:
			strncpy(s, "Unknown AF", maxlen);
			return NULL;
	}
	return s;
}
