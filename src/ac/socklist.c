/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>


#include "cw_log.h"
#include "sock.h"


#include "socklist.h"


struct socklistelem * socklist=0;
int socklist_len;
static pthread_mutex_t socklist_mutex;
static int socklist_wtpcount=0;


int socklist_init()
{

	if (pthread_mutex_init(&socklist_mutex,NULL))
		return 0;


	socklist = malloc(sizeof(struct socklistelem) * SOCKLIST_SIZE);
	memset(socklist,0,sizeof(struct socklistelem)*SOCKLIST_SIZE);
	if (!socklist){
		cw_log(LOG_ERR,"Fatal error while initializing socklist: %s",strerror(errno));
		return 0;
	}

	return 1;

}

void socklist_lock()
{
	pthread_mutex_lock(&socklist_mutex);
}

void socklist_unlock()
{
	pthread_mutex_unlock(&socklist_mutex);
}


void socklist_destroy()
{
	int i;
	for(i=0; i<socklist_len; i++){
		close(socklist[i].sockfd);
	}
	free(socklist);
	socklist=0;
	pthread_mutex_destroy(&socklist_mutex);

}


static int find_reply_socket(struct sockaddr *sa,int bc)
{
	int bestsockfd = -1;
	int i;
	for (i=0; i<socklist_len; i++){
		struct sockaddr_storage sn;
		memset(&sn,0,sizeof(sn));
		unsigned int snlen = sizeof(struct sockaddr_storage);

		if (getsockname(socklist[i].sockfd,(struct sockaddr*)&sn,&snlen)<0){
			continue;
		}

		if (sa->sa_family!=sn.ss_family)
			continue;

		if (bestsockfd == -1)
			bestsockfd = socklist[i].sockfd;

		if (!bc)
			return bestsockfd;

		struct sockaddr_storage bcaddr;

		if (!sock_getbroadcastaddr((struct sockaddr*)&sn,(struct sockaddr*)&bcaddr))
			continue;

		if (sock_cmpaddr((struct sockaddr*)&bcaddr,sa,0))
			continue;

		bestsockfd = socklist[i].sockfd;
		

	}	
	return bestsockfd;
}

void socklist_add_connection(int index)
{
	socklist_lock();
	socklist[index].wtpcount++;
	socklist_wtpcount++;
	socklist_unlock();
}

void socklist_del_connection(int index)
{
	socklist_lock();
	socklist[index].wtpcount--;
	socklist_wtpcount--;
	socklist_unlock();
}



int socklist_add_multicast(const char * addr, const char * port)
{

	struct addrinfo hints;
	struct addrinfo * res,*res0;
	memset(&hints,0,sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = PF_UNSPEC;
	hints.ai_flags=AI_PASSIVE;

	int rc = getaddrinfo(addr,port,&hints,&res0);
	if (rc!=0) {
		cw_log(LOG_ERR,"Can't bind multicast address '%s': %s",addr,gai_strerror(rc));
		return 0;
	}

	for(res=res0; res; res=res->ai_next){
		struct sockaddr *sa = res->ai_addr;
		int sockfd = socket(res->ai_addr->sa_family, SOCK_DGRAM, 0);
		/* create socket */
		if (sockfd==-1){
			cw_log(LOG_ERR,"Can't create multicast socket: %",strerror(errno));
			continue;
		}


		/* bind address */
		if ( bind(sockfd,sa,sock_addrlen(sa)) < 0) {
			close(sockfd);
			cw_log(LOG_ERR,"Can't bind  multicast %s: %s",addr,strerror(errno));
			continue;
		}

		/* use setsockopt() to request that the kernel joins a multicast group */
		void *opt;
		int optlen;
		if (res->ai_addr->sa_family == AF_INET){

			struct ip_mreq mreq;
			memset(&mreq,0,sizeof(mreq));
			struct sockaddr_in * sain = (struct sockaddr_in*)res->ai_addr;
			mreq.imr_multiaddr.s_addr=sain->sin_addr.s_addr;
			mreq.imr_interface.s_addr=htonl(INADDR_ANY);
			opt = &mreq;
			optlen=sizeof(mreq);

			char sinin[100];
			sock_addrtostr((struct sockaddr*)sain,sinin,100);

			if (setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,opt,optlen) < 0) {
				close(sockfd);
				cw_log(LOG_ERR,"Can't add multicast membership %s: %s",addr,strerror(errno));
				continue;
			}

		}
		if (res->ai_addr->sa_family == AF_INET6){
			struct ipv6_mreq mreq;
			memset(&mreq,0,sizeof(mreq));
			struct sockaddr_in6 * sain6 = (struct sockaddr_in6*)res->ai_addr;
//			mreq.ipv6mr_multiaddr.s_addr=sain->sin_addr.s_addr;
			memcpy(&mreq.ipv6mr_multiaddr.s6_addr,&sain6->sin6_addr.s6_addr,sizeof(sain6->sin6_addr.s6_addr));
//			int si  = sizeof(sain6->sin6_addr.s6_addr);

//			int i = sain6->sin6_addr.s6_addr;			
			mreq.ipv6mr_interface=0; //htonl(INADDR_ANY);
			opt = &mreq;
			optlen=sizeof(mreq);
			if (setsockopt(sockfd,IPPROTO_IPV6,IPV6_JOIN_GROUP,opt,optlen) < 0) {
				close(sockfd);
				cw_log(LOG_ERR,"Can't join multicast group %s: %s",addr,strerror(errno));
				continue;
			}
		}

		int rfd = find_reply_socket(sa,0);

		socklist[socklist_len].sockfd=sockfd;
		socklist[socklist_len].reply_sockfd=rfd;
		socklist[socklist_len].type=SOCKLIST_BCASTMCAST_SOCKET;
		socklist[socklist_len].family=sa->sa_family;

		socklist_len++;

		cw_log(LOG_INFO,"Bound to multicast group: %s (fd=%i,r:%i)",addr,sockfd,rfd);
	}

	freeaddrinfo(res0);
	return 1;
}


int socklist_add_unicast(const char *addr, const char * port) 
{
	struct addrinfo hints;
	struct addrinfo * res,*res0;
	memset(&hints,0,sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = PF_UNSPEC;
	hints.ai_flags=AI_PASSIVE;

	int rc = getaddrinfo(addr,port,&hints,&res0);
	if (rc!=0) {
		cw_log(LOG_ERR,"Can't bind multicast address '%s': %s",addr,gai_strerror(rc));
		return 0;
	}

	for(res=res0; res; res=res->ai_next){
		struct sockaddr *sa = res->ai_addr;
		int sockfd = socket(res->ai_addr->sa_family, SOCK_DGRAM, 0);
		/* create socket */
		if (sockfd==-1){
			cw_log(LOG_ERR,"Can't create unicast socket: %",strerror(errno));
			continue;
		}

		/* bind address */
		if ( bind(sockfd,sa,sock_addrlen(sa)) < 0) {
			close(sockfd);
			cw_log(LOG_ERR,"Can't bind  unicast socket %s: %s",addr,strerror(errno));
			continue;
		}


		socklist[socklist_len].sockfd=sockfd;
		socklist[socklist_len].reply_sockfd=sockfd;
		socklist[socklist_len].family=sa->sa_family;
		socklist[socklist_len].type=SOCKLIST_UNICAST_SOCKET;
	
		socklist_len++;
		cw_log(LOG_INFO,"Bound to: %s (%i)\n",addr,sockfd);
	}

	freeaddrinfo(res0);	
	return 1;
}

int socklist_add_broadcast(const char *addr, const char * port)
{
	struct addrinfo hints;
	struct addrinfo * res,*res0;
	memset(&hints,0,sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = PF_UNSPEC;
	hints.ai_flags=AI_PASSIVE;


	int rc = getaddrinfo(addr,port,&hints,&res0);
	if (rc!=0) {
		cw_log(LOG_ERR,"Can't bind broadcast address '%s': %s",addr,gai_strerror(rc));
		return 0;
	}

	int sockfd;
	for(res=res0; res; res=res->ai_next){

		struct sockaddr *sa = res->ai_addr;
		sockfd = socket(res->ai_addr->sa_family, SOCK_DGRAM, 0);

		/* create socket */
		if (sockfd==-1){
			cw_log(LOG_ERR,"Can't create broadcast socket: %",strerror(errno));
			continue;
		}

#ifdef IP_BINDANY
		struct sockaddr_in * sain = (struct sockaddr_in*)sa;
		if (sain->sin_addr.s_addr==INADDR_BROADCAST ){
			int opt=1;
			if (setsockopt(sockfd, IPPROTO_IP, IP_BINDANY, &opt, sizeof(opt))){
				cw_log(LOG_ERR,"Can't set sockopt IP_BIND_ANY: %s",strerror(errno));
				continue;
			};
		}
#endif		

		/* bind address */
		if ( bind(sockfd,sa,sock_addrlen(sa)) < 0) {
			close(sockfd);
			sockfd=-1;
			cw_log(LOG_ERR,"Can't bind  broadcast %s: %s",addr,strerror(errno));
			continue;
		}


		int rfd = find_reply_socket(sa,1);

		socklist[socklist_len].sockfd=sockfd;
		socklist[socklist_len].reply_sockfd=rfd;
		socklist[socklist_len].type=SOCKLIST_BCASTMCAST_SOCKET;
		socklist[socklist_len].family=sa->sa_family;
		socklist_len++;

		cw_log(LOG_INFO,"Bound to broadcast: %s (%i) (R:%i)\n",addr,sockfd,rfd);
	}

	freeaddrinfo(res0);	
	return 1;	
}


