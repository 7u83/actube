
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifdef AF_LINK
#   include <net/if_dl.h>
#endif
#ifdef AF_PACKET
#   include <netpacket/packet.h>
#endif

#include "sock.h"


int sock_getifhwaddr(const char *ifname, uint8_t * hwaddr, uint8_t * addrlen)
{
	struct sockaddr_storage sa;
	uint8_t *src;

#ifdef AF_PACKET	
	if (!sock_getifaddr(ifname,AF_PACKET,0,(struct sockaddr*)&sa))
		return 0;
	*addrlen = ((struct sockaddr_ll*)&sa)->sll_halen;
	src = ((struct sockaddr_ll*)&sa)->sll_addr;
#endif

#ifdef AF_LINK
	if (!sock_getifaddr(ifname,AF_LINK,0,(struct sockaddr*)&sa))
		return 0;
	struct sockaddr_dl *sdl = ((struct sockaddr_dl*)&sa);
	*addrlen = sdl->sdl_alen;
	src = ((uint8_t*)sdl->sdl_data)+sdl->sdl_nlen;
#endif


	memcpy(hwaddr,src,*addrlen);
	return 1;
}
