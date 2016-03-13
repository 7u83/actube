#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log.h"
#include "dbg.h"
#include "cw.h"

int cw_put_local_ip_address(int sock, uint8_t *dst, int ipv4elem_id, int ipv6elem_id)
{
	struct sockaddr_storage a;
	socklen_t alen = sizeof(struct sockaddr_storage);
	int rc = getsockname(sock, (struct sockaddr *) &a, &alen);

	if (rc != 0){
		cw_log(LOG_ERR,"Can't determine sock IP address - %s\n",strerror(errno));
		return 0;
	}


	int id; 

	switch (((struct sockaddr *) &a)->sa_family) {
		case AF_INET:
		{
			struct sockaddr_in *sain = (struct sockaddr_in *) &a;
			id  = ipv4elem_id;
			cw_put_data(dst + 4, (uint8_t *) & sain->sin_addr, 4);
			return 4 + cw_put_elem_hdr(dst, id, 4);
		}

		case AF_INET6:
		{
			id  = ipv6elem_id;
			struct sockaddr_in6 *sain = (struct sockaddr_in6 *) &a;
			cw_put_data(dst + 4, (uint8_t *) & sain->sin6_addr, 16);
			return 16 + cw_put_elem_hdr(dst, id, 16);

		}
	}

	cw_log(DBG_MSG_ERR, "Can't determine CAPWAP Local IP Adress");
	return 0;

}





