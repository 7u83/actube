/*
    This file is part of AC-Tube.

    AC-Tube is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AC-Tube is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "cw/capwap.h"
#include "cw/capwap_items.h"
#include "cw/log.h"
#include "cw/sock.h"
#include "cw/cw_util.h"
#include "cw/aciplist.h"
#include "cw/acpriolist.h"
#include "cw/timer.h"


#include "wtp.h"
#include "wtp_conf.h"
#include "wtp_interface.h"

#include <stdio.h>



cw_aciplist_t cw_select_ac(struct conn *conn, mbag_t discs)
{

	/* create a list for results */
	cw_aciplist_t resultlist=cw_aciplist_create();
	if (!resultlist)
		return NULL;
	if (!discs)
		return resultlist;

/*
	cw_aciplist_t aciplist = cw_aciplist_create();
	if (!aciplist) {
		cw_log(LOG_ERROR, "Can't allocate aciplist");
		return NULL;
	}

*/

	/* get the AC Name with Priority list */
	cw_acpriolist_t priolist;
	priolist = mbag_get_mavl(conn->config, CW_ITEM_AC_NAME_WITH_PRIORITY);
	if (!priolist )
		priolist=cw_acpriolist_create();


	/* for each discovery reponse */
	DEFINE_AVLITER(i, discs);
	avliter_foreach(&i){

		mbag_t ac = ((mbag_item_t *) (avliter_get(&i)))->data;

		/* get the ac name */
		char *ac_name = mbag_get_str(ac, CW_ITEM_AC_NAME,NULL);
		int prio = 256;

		if (ac_name) {
			/* See if we can find AC Name in Priority List */
			if (priolist)
				prio = cw_acpriolist_get(priolist, ac_name);
		}

		/* get the IP list, the current AC has sent */
		cw_aciplist_t acips =
		    mbag_get_mavl(ac, CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST);

		if (!acips)
			continue;

		/* for each IP from the current AC add it to the result list
		 * and give it the priority whe have determined */
		DEFINE_AVLITER(i2, acips);
		avliter_foreach(&i2){


			cw_acip_t *acip = avliter_get(&i2);

			cw_acip_t *n = malloc(sizeof(cw_acip_t));
			memcpy(n,acip,sizeof(cw_acip_t));
		
			/* we missuse the wtp_count to sort by 
			 * priority and wp_count */	
			n->wtp_count |= prio<<16; 

			cw_aciplist_del(resultlist,n);
			cw_aciplist_add(resultlist,n);
		}

	}

	return resultlist;
}



static int run_discovery(struct conn *conn)
{
//      conn->incomming = mbag_create();


	conn->capwap_state = CW_STATE_DISCOVERY;
	mbag_set_byte(conn->outgoing, CW_ITEM_DISCOVERY_TYPE,
			      CW_DISCOVERY_TYPE_UNKNOWN);


	cw_init_request(conn, CW_MSG_DISCOVERY_REQUEST);
	cw_put_msg(conn, conn->req_buffer);
	conn_send_msg(conn, conn->req_buffer);


	time_t timer = cw_timer_start(0);


	while (!cw_timer_timeout(timer)
	       && conn->capwap_state == CW_STATE_DISCOVERY) {
		mavl_del_all(conn->incomming);

		int rc = cw_read_from(conn);

		if (rc<0) {
			if (errno==EAGAIN)
				continue;

			cw_log(LOG_ERROR,"Error reading messages: %s",strerror(errno));
			break;
		}
	}


	mbag_t discs;
	discs = mbag_get_mavl(conn->remote, CW_ITEM_DISCOVERIES);


	if (!discs) {
		cw_log(LOG_ERR,"No discovery responses received");
		return 0;
	}

	int i;

	cw_aciplist_t list = cw_select_ac(conn, discs);



	DEFINE_AVLITER(ii,list);
	avliter_foreach(&ii){
		cw_acip_t * ip = avliter_get(&ii);

	}

	

	mavl_del_all(conn->remote);

	mbag_set_mavl(conn->local,CW_ITEM_CAPWAP_CONTROL_IP_ADDRESS_LIST,list);

	return 1;
}




/**
 * Run discovery for on address (eg broadcast 255.255.255.255)
 */
static int cw_run_discovery(struct conn *conn, const char *acaddr)
{

	/* get addr of destination */
	struct addrinfo hints;
	struct addrinfo *res, *res0;
	memset(&hints, 0, sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = PF_UNSPEC;

	int rc = getaddrinfo(acaddr, conf_control_port, &hints, &res0);
	if (rc) {
		cw_log(LOG_ERR, "Can't connect to AC %s: %s", acaddr, gai_strerror(rc));
		return 0;
	}

	for (res = res0; res; res = res->ai_next) {

		int sockfd;
		int opt;
		sockfd = socket(res->ai_family, SOCK_DGRAM, 0);
		if (sockfd == -1) {
			cw_log(LOG_ERR, "Can't create socket for %s: %s", acaddr,
			       strerror(errno));
			continue;
		}

		opt = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)) < 0) {
			cw_log(LOG_ERR, "Can't set broadcast sockopt");
		}
		sock_set_recvtimeout(sockfd, 1);
		sock_set_dontfrag(sockfd, 0);

		sock_copyaddr(&conn->addr, res->ai_addr);
		conn->sock = sockfd;
		conn->readfrom = conn_recvfrom_packet;

		run_discovery(conn);

		conn->readfrom=NULL;
		close(sockfd);


	}

	freeaddrinfo(res0);

	return 0;
}


int discovery()
{
	struct conn *conn = get_conn();

	printf("Radios = %d\n",conn->radios->count);

	cw_run_discovery(conn, "255.255.255.255");
	conn->capwap_state=CW_STATE_JOIN;
	return 1;
}
