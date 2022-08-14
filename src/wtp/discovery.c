
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "cw/capwap.h"
#include "cw/conn.h"
#include "cw/sock.h"
#include "cw/val.h"
#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/timer.h"
#include "cw/cw.h"
#include "cw/rand.h"

#include "wtp.h"
#include "cw/cfg.h"

#include "cw/discovery.h"




static int discovery_cb(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len)
{
	struct cw_DiscoveryResults *results = (struct cw_DiscoveryResults *)params->conn->data;
//	cw_Cfg_t * cfg = cw_cfg_create();
//	cw_cfg_copy(params->cfg,cfg);

	cw_discovery_results_add(results,params->cfg,params->conn->global_cfg);
	printf("Have Discovery %d\n",results->nr);

//	cw_cfg_dump(params->cfg);
//	mlist_append_ptr(dis->results, cfg);
	return 0;
}

static struct cw_DiscoveryResults * run_discovery(struct cw_Conn *conn)
{
	time_t timer;
	struct sockaddr_storage from;
	int delay, min, max;

	struct cw_DiscoveryResults * results;
	results = cw_discovery_results_create();

	
	min = cw_cfg_get_byte(conn->local_cfg,"capwap-timers/min-discovery-interval",
					CAPWAP_MIN_DISCOVERY_INTERVAL);
	max = cw_cfg_get_byte(conn->local_cfg,"capwap-timers/max-discovery-interval",
					CAPWAP_MAX_DISCOVERY_INTERVAL);

	delay = cw_randint(min,max);
	if (delay>0){
		cw_dbg(DBG_INFO,"Sleeping for %d seconds before sending Discovery Request.", delay);
		sleep(delay);
	}
	

	conn->capwap_state = CAPWAP_STATE_DISCOVERY;

	conn->remote_cfg=cw_cfg_create();
	/* create and send a discovery request message */
	cw_init_request(conn, CAPWAP_MSG_DISCOVERY_REQUEST);
	cw_assemble_message(conn, conn->req_buffer);


	conn_send_msg(conn, conn->req_buffer);
	cw_cfg_destroy(conn->remote_cfg);
	conn->remote_cfg=NULL;




	timer = cw_timer_start(cw_cfg_get_byte(conn->local_cfg, "discovery-interval",
					       CAPWAP_DISCOVERY_INTERVAL))-1;

	/*discovery_results = mlist_create(NULL, NULL, sizeof(void *));*/

	conn->data = results;
	cw_conn_set_msg_cb(conn,CAPWAP_MSG_DISCOVERY_RESPONSE,discovery_cb);

	
	while (!cw_timer_timeout(timer)
	       && conn->capwap_state == CAPWAP_STATE_DISCOVERY) {
		int rc;
		cw_dbg(DBG_X,"READ NOW");
//		conn->remote_cfg = cw_ktv_create();
//		if (conn->remote_cfg == NULL) {
//			cw_log_errno("Can't allocate memory for remote_cfg");
//			break;
//		}

		rc = cw_read_from(conn, &from);
		if (rc < 0) {
			if (errno == EAGAIN)
				continue;

			cw_log(LOG_ERROR, "Error reading messages: %s", strerror(errno));
			break;
		}
//		cw_dbg(DBG_INFO, "Received Discovery Response from %s",
//		       sock_addr2str(&from, addr_str));
//		mlist_append_ptr(dis->results, conn->remote_cfg);

//		cw_cfg_dump(conn->remote_cfg);
	}


	//cw_select_ac(conn->local_cfg, dis);

	mavliter_t it;
        mavliter_init(&it,results->list);
        mavliter_foreach(&it){
		const char * acname;
		struct cw_DiscoveryResults_elem *e = mavliter_get(&it);
		acname = cw_cfg_get(e->cfg,"capwap/ac-name","<unknown>");
		printf("E: %s: %s - prio: %d, ctr: %d\n",acname,e->ip,e->prio,e->ctr);

	}

	
	return results;
}




/**
 * Run discovery for on address (eg broadcast 255.255.255.255)
 */
struct cw_DiscoveryResults * cw_run_discovery(struct cw_Conn *conn, const char *addr, const char *bindaddr )
{
	struct cw_DiscoveryResults * results;
	char sock_buf[SOCK_ADDR_BUFSIZE];
	struct sockaddr_storage dstaddr;
	char caddr[256], control_port[64];

	int rc;
	
	
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

		results = run_discovery(conn);

		conn->readfrom = NULL;
		close(sockfd);


	}

	freeaddrinfo(res0);

	return results;
}
