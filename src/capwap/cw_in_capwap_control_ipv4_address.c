

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#include <errno.h>

#include "action.h"
#include "log.h"
#include "itemstore.h"
#include "capwap.h"
#include "capwap_items.h"
#include "aciplist.h"
#include "sock.h"



int cw_in_capwap_control_ipv4_address(struct conn *conn, struct cw_action_in *a,
				 uint8_t * data, int len)
{
	cw_aciplist_t list =
	    cw_itemstore_get_avltree_c(conn->incomming,a->item_id,cw_aciplist_create);

	if (!list) {
		cw_log(LOG_ERR, "Error: Can't allocate CAWAP IP Adress List");
		return 0;
	}

	cw_acip_t * acip;
	acip = malloc(sizeof(cw_acip_t));
	if (!acip) {
		cw_log(LOG_ERR,"Can't allocate memory for acv4ip: %s",strerror(errno));
			return 0;
	}

	struct sockaddr_in addr;
	memcpy(&addr.sin_addr,data,4);
	addr.sin_family=AF_INET;
	sock_setport((struct sockaddr*)&addr,CAPWAP_CONTROL_PORT);
	memcpy(&acip->ip,&addr,sizeof(addr));

	acip->wtp_count = cw_get_word(data+4);
	cw_aciplist_replace(list,acip);


	return 1;
}
