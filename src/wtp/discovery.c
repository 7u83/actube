
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

#include "wtp.h"


int cw_select_ac(mavl_t local_cfg,mlist_t aclist){
	mlistelem_t * e;
	int en;
	mavl_t iplist;
	
	iplist=cw_ktv_create();
	if (iplist == NULL)
		return 0;

	en=0;

	/* for each discovery response */
	mlist_foreach(e,aclist){
		char str[1024];
		char key[CW_KTV_MAX_KEY_LEN];
		mavl_t remote_cfg;
		int i;
		cw_KTV_t * val;
		int prio;
		
		remote_cfg = mlistelem_get_ptr(e);
		
		/* get ac name */
		val = cw_ktv_get(remote_cfg,"ac-name", CW_TYPE_BSTR16);
		if (val==NULL)
			continue;
		
		val->type->to_str(val,str,1024);
		sprintf(key,"ac-name-with-priority/%s",str);
		
		printf("Get prio: %s\n",key);
		
		prio = cw_ktv_get_byte(local_cfg,key,255);
		
		i=0; 
		do {
			cw_KTV_t * ipval;
			sprintf(key,"%s.%d","capwap-control-ip-address/wtps",i);
			val = cw_ktv_get(remote_cfg,key,CW_TYPE_WORD);
			if (val == NULL)
				break;
			
			sprintf(key,"%s.%d","capwap-control-ip-address/address",i);
			printf("ipvalkey: %s\n",key);
			ipval = cw_ktv_get(remote_cfg,key,CW_TYPE_IPADDRESS);

			sprintf(key,"%04d%05d%04d",prio,val->val.word,en);
			en++;
			printf("This is the key: %s\n",key);
			
			cw_ktv_add(iplist,key,CW_TYPE_SYSPTR,(uint8_t*)(&ipval),sizeof(ipval));
			i++;	
		}while(1);
		printf("Here we have an AC: %s\n",str);
		
		
	}
	
	cw_dbg_ktv_dump(iplist,DBG_INFO,"=== IP list ===", "IP", "=== END IP List ===");
	{
		mavliter_t i;
		mavliter_init(&i,iplist);
		
		mavliter_foreach(&i){
			char ipstr[100];
			char *rk;
			cw_KTV_t * val;
			val = mavliter_get(&i);
			rk = val->key;
			val = val->val.ptr;
			val->type->to_str(val,ipstr,100);
			printf("PTRVAL(%s): %s - %s\n",rk,val->key,ipstr);
		}
	}
	
	return 0;
}

static int run_discovery(struct conn *conn)
{
/*//      conn->incomming = mbag_create();*/
	time_t timer;
/*	uint8_t dtype=0;*/
	mlist_t discovery_results;
	struct sockaddr_storage from;
	
	conn->capwap_state = CAPWAP_STATE_DISCOVERY;
	
	
	/*cw_ktv_add(conn->local_cfg, "discovery-type", CW_TYPE_BYTE, &dtype,17);*/


	cw_init_request(conn, CAPWAP_MSG_DISCOVERY_REQUEST);
	cw_put_msg(conn, conn->req_buffer);
	conn_send_msg(conn, conn->req_buffer);


	timer = cw_timer_start(0);

	
	discovery_results = mlist_create(NULL,NULL,sizeof(void*));
	while (!cw_timer_timeout(timer)
	       && conn->capwap_state == CAPWAP_STATE_DISCOVERY) {
		int rc;
		char addr_str[SOCK_ADDR_BUFSIZE];
		/*mavl_del_all(conn->incomming);*/

		rc = cw_read_from(conn, &from);
		if (rc<0) {
			if (errno==EAGAIN)
				continue;

			cw_log(LOG_ERROR,"Error reading messages: %s",strerror(errno));
			break;
		}
		cw_dbg(DBG_INFO,"Received Discovery Response from %s", sock_addr2str(&from,addr_str));
		mlist_append_ptr(discovery_results,conn->remote_cfg);
		conn->remote_cfg=cw_ktv_create();
	}

	cw_select_ac(conn->local_cfg,discovery_results);
	
	
/*
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
*/
	return 1;
}

/**
 * Run discovery for on address (eg broadcast 255.255.255.255)
 */
int cw_run_discovery(struct conn *conn, const char *addr, const char *bindaddr)
{
	char sock_buf[SOCK_ADDR_BUFSIZE];
	struct sockaddr_storage dstaddr;
	char caddr[256], control_port[64];
	
	int rc;
	
	/* get addr of destination */
	struct addrinfo hints;
	struct addrinfo *res, *res0;
	

	
	memset(&hints, 0, sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_UNSPEC;
	
	
	strncpy(caddr, addr, sizeof(caddr));
	
	/* try to convert the given address string to sockaddr */
	rc = sock_strtoaddr(caddr,(struct sockaddr*)&dstaddr);
	if (rc) {
		int iport;
		/* converting to socaddr was successful */	
		iport = sock_getport((struct sockaddr*)&dstaddr);
		if (iport != 0)
			sprintf(control_port, "%d", iport);
		else
			sprintf(control_port,"%d", CAPWAP_CONTROL_PORT);
	}
	else{
		char * port;
		/* converting went wrong, so the address string might
		 * be a DNS entry. Look for a colon to find the port. */
		port = strchr(addr,':');
		if (port){
			strncpy(control_port,port+1,sizeof(control_port));
			caddr[port-addr]=0;
		}
		else{
			sprintf(control_port,"%d", CAPWAP_CONTROL_PORT);
		}
	}
	

	/*printf("Addr: %s, Port: %s\n",caddr,control_port);*/

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

		
		if (bindaddr){
			int brc;
			
			struct sockaddr bind_address;
			sock_strtoaddr(bindaddr,&bind_address); 
			brc = bind(sockfd,&bind_address,sock_addrlen(&bind_address));
			if (brc<0) {
				cw_log(LOG_ERR,"Can't bind to %s",sock_addr2str(&bind_address,sock_buf));
				return 0;
			}
		}
		

		conn->sock = sockfd;
		conn->readfrom = conn_recvfrom_packet;

		
		cw_dbg(DBG_INFO,"Discovery to %s", sock_addr2str_p(&conn->addr,sock_buf));

		run_discovery(conn);

		conn->readfrom=NULL;
		close(sockfd);


	}

	freeaddrinfo(res0);

	return 0;
}



