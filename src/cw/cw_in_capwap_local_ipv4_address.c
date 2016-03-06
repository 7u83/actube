
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "cw.h"
#include "capwap_items.h"
#include "mbag.h"


int cw_in_capwap_local_ipv4_address(struct conn *conn, struct cw_action_in *a,
				 uint8_t * data, int len,struct sockaddr *from)
{
	struct sockaddr_in addr;
	memcpy(&addr.sin_addr,data,4);
	addr.sin_family=AF_INET;

	mbag_set_sockaddr(conn->incomming,CW_ITEM_CAPWAP_LOCAL_IP_ADDRESS,(struct sockaddr*)&addr);
	
	
	return 1;
}


