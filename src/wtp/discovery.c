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


#include "capwap/capwap.h"
#include "capwap/cw_log.h"
#include "capwap/wtpinfo.h"
#include "capwap/acinfo.h"
#include "capwap/sock.h"
#include "capwap/cw_util.h"

#include "wtp.h"
#include "wtp_conf.h"
#include "wtp_interface.h"

#include <stdio.h>

struct discovery_info{
	ACIPLIST * aciplist;
	struct conn * conn;
	int response_count;
};

static int acprint(void *p,void*d) //,int ctr)
{


	ACIP * ip = (ACIP*)d;
	char str[100];
	sock_addrtostr((struct sockaddr*)&ip->ip,str,100);
//	printf("ACIP: %s\n",str);
//	printf("CTR: %i\n",ip->wtp_count);
	return 1;
}




static int msg_cb(void *priv,struct cwrmsg * cwrmsg)
{
	if (cwrmsg->type != CWMSG_DISCOVERY_RESPONSE){
		cw_dbg(DBG_ERR,"Expected discovery response, but received %i",cwrmsg->type);
		return 1;
	}

	struct discovery_info * di = (struct discovery_info*)priv;

/*
	if (di->conn->seqnum != cwrmsg->seqnum){
		cw_log_debug0("Ignoring discovery respone, seqnum=%i, expected seqnum %i",cwrmsg->seqnum,di->conn->seqnum);
		return 1;
	}
*/


	struct ac_info acinfo;
	memset(&acinfo,0,sizeof(acinfo));

	acinfo.aciplist = di->aciplist;




	cwread_discovery_response(&acinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);





	char ai [4096];
	acinfo_print(ai,&acinfo);
	printf("AC INFO\n%s",ai);


//printf("CWRHW: %s\n",sock_hwaddr2str(bstr_data(cwrmsg->rmac),bstr_len(cwrmsg->rmac)));
///exit(0);


/*
	printf("AC NAME %s\n",acinfo.ac_name);
	printf("AC Software version %s\n",acinfo.software_version);
	printf("AC Hardwareversion %s\n",acinfo.hardware_version);

	printf("Active wtps: %i\n",acinfo.active_wtps);
	printf("Active max wtps: %i\n",acinfo.max_wtps);


	printf("Got Discovery response\n");

	printf("ACL COUNT: %i\n",acinfo.aciplist->count);
*/	
	aciplist_foreach(acinfo.aciplist,acprint,0);

//	responses++;
//
	di->response_count++;	
	return 1;	
}


static void rand_sleep(int seconds)
{
	int usecs = seconds * 1000;
	uint16_t rnd;
	cw_rand((uint8_t*)&rnd,sizeof(rnd));
	uint16_t max = 0-1; 
	int r = (rnd * usecs) / max;
	cw_dbg(DBG_CW_INFO,"Sleeping for %u milliseconds\n",r);
	usleep(r*1000);
}


static int do_discover_conn(struct conn * conn,struct discovery_info * di)
{
	rand_sleep(conf_max_discovery_interval);


	struct wtpinfo *  wtpinfo;
	wtpinfo = get_wtpinfo();
//	wtpinfo_print(wtpinfo);


//	struct timespec tstart,tcur;
	struct radioinfo ri;
	memset(&ri,0,sizeof(ri));

	struct radioinfo * rip = &wtpinfo->radioinfo[0];

/*
	ri.rmac[0]=6;
	ri.rmac[2]=14;
	ri.rmac[3]=14;
	ri.rmac[4]=14;
	ri.rmac[5]=14;
	ri.rmac[6]=14;
	ri.rmac[7]=14;
*/

#ifdef WITH_CW_LOG_DEBUG
	char str[100];
	sock_addrtostr((struct sockaddr*)&conn->addr,str,100);
//	cw_log_debug0("Sending discovery request to %s",str);
#endif

	int rc;

	do {
		conn->capwap_mode=CWMODE_CISCO;
		rc = cwsend_discovery_request(conn,rip,wtpinfo);
		if (rc<0){
			if (errno == EINTR)
				continue;
			if (errno == EMSGSIZE){
				conn->mtu-=4;
//				cw_log_debug2("Setting mtu to %i",conn->mtu);
				continue;
			}
		}
		break;

	}while (rc<0);
	

	if (rc < 0 )
	{
		char str[100];
		sock_addrtostr((struct sockaddr*)&conn->addr,str,100);
		cw_log(LOG_ERR,"Error sending discovery request to %s: %s",str,strerror(errno));
		return 0;
	}

	struct connlist * connlist;
	connlist = connlist_create(30);

//	clock_gettime(CLOCK_REALTIME,&tstart);	
//
	int tstart = time(0);	
	int treset = 0;
	do {
		char buf[2048];
		int  buflen=2048;
		struct sockaddr_storage sa;
		socklen_t fromlen=sizeof(struct sockaddr_storage);
		rc = recvfrom(conn->sock,buf,buflen,0,(struct sockaddr*)&sa,&fromlen);
		if (rc<0){
			if (errno==EINTR)
				rc=0;
			if (errno==EAGAIN)
				rc=0;
			if (errno==EWOULDBLOCK)
				rc=0;
		}
		if (rc>0) {
#ifdef WITH_CW_LOG_DEBUG
			char str[100];
			sock_addrtostr((struct sockaddr*)&sa,str,100);
//			cw_log_debug0("Received packet from %s",str);
#endif

			struct conn * rconn; 
			rconn = connlist_get(connlist,(struct sockaddr*)&sa);

			if (!rconn){
				rconn = conn_create_noq(conn->sock,(struct sockaddr*)&sa); //msg_cb,NULL,0);
	//			rconn->pmsgarg=conn->pmsgarg;
				rconn->mtu = conn->mtu;
				rconn->seqnum=conn->seqnum;
				connlist_add(connlist,rconn);
			}

			conn_process_packet(rconn,(uint8_t*)buf,rc,msg_cb,di);
		}

		/* reset discovery timer after we have received the first response */
		if ( di->response_count == 1 && !treset ){
			tstart=time(0);
			treset=1;
		}

		//clock_gettime(CLOCK_REALTIME,&tcur);
//		printf("TTSub: %i %i\n",time(0)-tstart, conf_discovery_interval);
	}while(time(0)-tstart < conf_discovery_interval && rc>=0 );			


	if (rc <0){
		char str[100];
		sock_addrtostr((struct sockaddr*)&conn->addr,str,100);
		cw_log(LOG_ERR,"Error sendings discovery request to %s: %s",str,strerror(errno));
	}

	connlist_destroy(connlist);
	return 1;
}


static int discovery_count;

ACIPLIST * do_discovery(const char *acaddr)
{

	/* get an partially intialized connection object 
	 * (seqnum should be set)
	 * */
	struct conn * conn = get_conn();
	if (!conn){
		cw_log(LOG_ERR,"Can't create conn for %s: %s",acaddr,strerror(errno));
		return 0;
	}

     
       /* get addr of destination */	
	struct addrinfo hints;
	struct addrinfo * res,*res0;
	memset(&hints,0,sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = PF_UNSPEC;

	int rc = getaddrinfo(acaddr,conf_control_port,&hints,&res0);
	if(rc){
		cw_log(LOG_ERR,"Can't connect to AC %s: %s",acaddr,gai_strerror(rc));
		return 0;
	}

	struct discovery_info di;
	memset (&di,0,sizeof(struct discovery_info));
	di.aciplist = aciplist_create();
	di.response_count=0;


	for(res=res0; res; res=res->ai_next)
	{
		if ( discovery_count >= conf_max_discoveries){
			sulking_state();
			discovery_count=0;
	//		cw_log_debug0("Entering discovery state");
		}
		discovery_count++;


		/* create socket */
		int sockfd;
		int opt;
		sockfd=socket(res->ai_family,SOCK_DGRAM,0);
		if (sockfd == -1){
			cw_log(LOG_ERR,"Can't create socket for %s: %s",acaddr,strerror(errno));
			continue;
		}
	
		opt = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt))<0){
			cw_log(LOG_ERR,"Cant set broadcast sockopt");
		}
		sock_set_recvtimeout(sockfd,1);

		sock_set_dontfrag(sockfd,0);

		di.conn=conn;

		sock_copyaddr(&conn->addr,res->ai_addr);
		conn->sock=sockfd;

		do_discover_conn(conn,&di);	

		close(sockfd);

		if ( di.aciplist->count != 0)
			break;
		
	}

	freeaddrinfo(res0);

	if (di.aciplist->count){
//		cw_log_debug2("Discover responses received: %i\n",di.response_count);
		return di.aciplist;

	}

	aciplist_destroy(di.aciplist);	
	return 0;
}


