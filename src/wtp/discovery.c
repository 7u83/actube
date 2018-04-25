
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "cw/capwap.h"
#include "cw/conn.h"
#include "cw/sock.h"
#include "cw/ktv.h"
#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/timer.h"
#include "cw/cw.h"
#include "cw/rand.h"

#include "wtp.h"





int cw_select_ac(mavl_t local_cfg, struct cw_DiscoveryResult * dis)
{
	mlistelem_t *e;
	/*mavl_t iplist;*/
	int en;

	/*iplist = cw_ktv_create();
	if (iplist == NULL){
		cw_log_errno("cw_select_ac: Can't allocate memory for aciplist");
		return NULL;
	}
	*/

	en = 0;

	/* for each discovery response */
	mlist_foreach(e, dis->results) {
		char acname[CAPWAP_MAX_AC_NAME_LEN + 1];
		char key[CW_KTV_MAX_KEY_LEN];
		mavl_t remote_cfg;
		cw_KTV_t *val, *ipval;
		int prio, i;

		remote_cfg = mlistelem_get_ptr(e);

		/* get ac name */
		val = cw_ktv_get(remote_cfg, "ac-name", CW_TYPE_BSTR16);
		if (val == NULL) {
			/* this should not happen, because AC Name is a
			 * amndatory message element */
			prio = 255;
		} else {
			/* Get priority for AC from 
			 * ac-name-with-priority list */
			val->type->to_str(val, acname, CAPWAP_MAX_AC_NAME_LEN);
			sprintf(key, "ac-name-with-priority/%s", acname);
			prio = cw_ktv_get_byte(local_cfg, key, 255);
		}

		/* for each control ip address the AC has sent */
		i = 0;
		do {
			sprintf(key, "%s.%d", "capwap-control-ip-address/wtps", i);
			val = cw_ktv_get(remote_cfg, key, CW_TYPE_WORD);
			if (val == NULL)
				break;

			sprintf(key, "%s.%d", "capwap-control-ip-address/address", i);
			ipval = cw_ktv_get(remote_cfg, key, CW_TYPE_IPADDRESS);

			sprintf(key, "%04d%05d%04d", prio, val->val.word, en);

			cw_ktv_add(dis->prio_ip, key, CW_TYPE_SYSPTR, NULL, (uint8_t *) (&ipval), 
				   sizeof(ipval));
			cw_ktv_add(dis->prio_ac, key, CW_TYPE_SYSPTR, NULL, (uint8_t *)(&remote_cfg),
				sizeof(remote_cfg));
			i++;
			en++;
		} while (1);

	}



	return 1;
}


void cw_discovery_free_results(struct cw_DiscoveryResult * dis)
{
	if (dis->prio_ac != NULL)
		mavl_destroy(dis->prio_ac);
	if (dis->prio_ip != NULL)
		mavl_destroy(dis->prio_ip);
	if (dis->results != NULL)
		mlist_destroy(dis->results);
}


static void result_del(void * data)
{
	
	mavl_t todelete = *((void**)data);
	mavl_destroy(todelete);
	
	
	/*mavl_t*/
}

int cw_discovery_init_results(struct cw_DiscoveryResult *dis)
{
	
	dis->results = mlist_create(NULL, result_del, sizeof(void *));
	if (dis->results==NULL)
		goto errX;
	dis->prio_ac=cw_ktv_create();
	if (dis->prio_ac==NULL)
		goto errX;
	dis->prio_ip=cw_ktv_create();
	if (dis->prio_ac==NULL)
		goto errX;
	return 1;
errX:
	cw_discovery_free_results(dis);
	return 0;
}


static int run_discovery(struct conn *conn, struct cw_DiscoveryResult * dis)
{
	time_t timer;
	struct sockaddr_storage from;
	int delay, min, max;

	
	
	min = cw_ktv_get_byte(conn->local_cfg,"capwap-timers/min-discovery-interval",
					CAPWAP_MIN_DISCOVERY_INTERVAL);
	max = cw_ktv_get_byte(conn->local_cfg,"capwap-timers/max-discovery-interval",
					CAPWAP_MAX_DISCOVERY_INTERVAL);

	delay = cw_randint(min,max);
	if (delay>0){
		cw_dbg(DBG_INFO,"Sleeping for %d seconds before sending Discovery Request.", delay);
		sleep(delay);
	}
	

	conn->capwap_state = CAPWAP_STATE_DISCOVERY;


	/* create and send a discovery request message */
	cw_init_request(conn, CAPWAP_MSG_DISCOVERY_REQUEST);
	cw_put_msg(conn, conn->req_buffer);
	conn_send_msg(conn, conn->req_buffer);


	timer = cw_timer_start(cw_ktv_get_byte(conn->local_cfg, "discovery-interval",
					       CAPWAP_DISCOVERY_INTERVAL))-1;

	/*discovery_results = mlist_create(NULL, NULL, sizeof(void *));*/
	
	
	while (!cw_timer_timeout(timer)
	       && conn->capwap_state == CAPWAP_STATE_DISCOVERY) {
		int rc;
		char addr_str[SOCK_ADDR_BUFSIZE];

		conn->remote_cfg = cw_ktv_create();
		if (conn->remote_cfg == NULL) {
			cw_log_errno("Can't allocate memory for remote_cfg");
			break;
		}

		rc = cw_read_from(conn, &from);
		if (rc < 0) {
			if (errno == EAGAIN)
				continue;

			cw_log(LOG_ERROR, "Error reading messages: %s", strerror(errno));
			break;
		}
		cw_dbg(DBG_INFO, "Received Discovery Response from %s",
		       sock_addr2str(&from, addr_str));
		mlist_append_ptr(dis->results, conn->remote_cfg);
	}

	cw_select_ac(conn->local_cfg, dis);



	return 1;
}




/**
 * Run discovery for on address (eg broadcast 255.255.255.255)
 */
int cw_run_discovery(struct conn *conn, const char *addr, const char *bindaddr, 
		struct cw_DiscoveryResult * dis)
{
	char sock_buf[SOCK_ADDR_BUFSIZE];
	struct sockaddr_storage dstaddr;
	char caddr[256], control_port[64];

	int rc;
	/*struct cw_DiscoveryResult dis;*/
	
	
	/*dis.results = mlist_create(NULL, NULL, sizeof(void *));*/
	

	/* get addr of destination */
	struct addrinfo hints;
	struct addrinfo *res, *res0;



	memset(&hints, 0, sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_UNSPEC;


	strncpy(caddr, addr, sizeof(caddr));

	/* try to convert the given address string to sockaddr */
	rc = sock_strtoaddr(caddr, (struct sockaddr *) &dstaddr);
	if (rc) {
		int iport;
		/* converting to socaddr was successful */
		iport = sock_getport((struct sockaddr *) &dstaddr);
		if (iport != 0)
			sprintf(control_port, "%d", iport);
		else
			sprintf(control_port, "%d", CAPWAP_CONTROL_PORT);
	} else {
		char *port;
		/* converting went wrong, so the address string might
		 * be a DNS entry. Look for a colon to find the port. */
		port = strchr(addr, ':');
		if (port) {
			strncpy(control_port, port + 1, sizeof(control_port));
			caddr[port - addr] = 0;
		} else {
			sprintf(control_port, "%d", CAPWAP_CONTROL_PORT);
		}
	}


	/*printf("Addr: %s, Port: %s\n",caddr,control_port); */

	rc = getaddrinfo(caddr, control_port, &hints, &res0);
	if (rc) {
		cw_log(LOG_ERR, "Can't connect to AC %s: %s", caddr, gai_strerror(rc));
		return 0;
	}

	for (res = res0; res; res = res->ai_next) {

		int sockfd;
		int opt;
		sockfd = socket(res->ai_family, SOCK_DGRAM, 0);
		if (sockfd == -1) {
			cw_log(LOG_ERR, "Can't create socket for %s: %s", caddr,
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


		if (bindaddr) {
			int brc;

			struct sockaddr bind_address;
			sock_strtoaddr(bindaddr, &bind_address);
			brc = bind(sockfd, &bind_address, sock_addrlen(&bind_address));
			if (brc < 0) {
				cw_log(LOG_ERR, "Can't bind to %s",
				       sock_addr2str(&bind_address, sock_buf));
				return 0;
			}
		}


		conn->sock = sockfd;
		conn->readfrom = conn_recvfrom_packet;


		cw_dbg(DBG_INFO, "Discovery to %s",
		       sock_addr2str_p(&conn->addr, sock_buf));

		run_discovery(conn, dis);

		conn->readfrom = NULL;
		close(sockfd);


	}

	freeaddrinfo(res0);

	return 0;
}
