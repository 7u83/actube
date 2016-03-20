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
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "cw/log.h"
#include "cw/sock.h"


#include "socklist.h"


struct socklistelem *socklist = 0;
int socklist_len;
static pthread_mutex_t socklist_mutex;
static int socklist_wtpcount = 0;


int socklist_init()
{

	if (pthread_mutex_init(&socklist_mutex, NULL))
		return 0;


	socklist = malloc(sizeof(struct socklistelem) * SOCKLIST_SIZE);
	memset(socklist, 0, sizeof(struct socklistelem) * SOCKLIST_SIZE);
	if (!socklist) {
		cw_log(LOG_ERR, "Fatal error while initializing socklist: %s",
		       strerror(errno));
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
	for (i = 0; i < socklist_len; i++) {
		close(socklist[i].sockfd);
	}
	free(socklist);
	socklist = 0;
	pthread_mutex_destroy(&socklist_mutex);
}

/**
 * Find a good reply socket (only for IPv4)
 * @param sa source address
 * @return socket or -1 if no socket was found
 */
int socklist_find_reply_socket(struct sockaddr *sa, int port)
{
	int bestindex = -1;
	int i;


	for (i = 0; i < socklist_len; i++) {
		/* we want only an unicast socket for reply */
		if (socklist[i].type != SOCKLIST_UNICAST_SOCKET)
			continue;

		/* and we want only sockets with same sa_family */
		if (sa->sa_family != socklist[i].addr.sa_family)
			continue;


		/* the first fd would be always the best, if we don't
		 * find later a better one */
		if (bestindex == -1) {
			bestindex = i;
			continue;
		}

		/* we want first the same port */
		if (sock_getport(&socklist[i].addr) != port) {
			continue;
		}

		/* if we havn't already found a socket with same port
		 * this is now our best socket*/
		if (sock_getport(&socklist[bestindex].addr) != port) {
			bestindex = i;
		}

		/* the next checks only aply to IPv4 */
		if (socklist[i].addr.sa_family != AF_INET)
			continue;

		/* get our source address and netmask */
		uint32_t addr =
		    ((struct sockaddr_in *) &socklist[i].addr)->sin_addr.s_addr;
		uint32_t mask =
		    ((struct sockaddr_in *) &socklist[i].netmask)->sin_addr.s_addr;

		/* get source of requested address */
		uint32_t saddr = ((struct sockaddr_in *) sa)->sin_addr.s_addr;

		/* if the request comes from the same subnet where our
		 * socket is cconnected, this is our new best socked.
		 * So we can serve AP's w*here no deault route is configured */
		if ((addr & mask) == (saddr & mask)) {
			bestindex = i;
		}
	}

	if (bestindex != -1)
		return socklist[bestindex].sockfd;
	return -1;
}

static int find_reply_socket(struct sockaddr *sa, int bc)
{

	//printf("Looking for best sock of: %s\n",sock_addr2str(sa));

	int bestsockfd = -1;
	int i;
	for (i = 0; i < socklist_len; i++) {
		struct sockaddr_storage sn;
		memset(&sn, 0, sizeof(sn));
		unsigned int snlen = sizeof(struct sockaddr_storage);

		if (getsockname(socklist[i].sockfd, (struct sockaddr *) &sn, &snlen) < 0) {
			continue;
		}

		if (sa->sa_family != sn.ss_family)
			continue;

		if (sn.ss_family == AF_INET) {
			int p1 = ntohs(((struct sockaddr_in *) sa)->sin_port);
			int p2 = ntohs(((struct sockaddr_in *) &sn)->sin_port);
			if (p1 != p2)
				continue;

		}



		if (bestsockfd == -1)
			bestsockfd = socklist[i].sockfd;

		if (!bc)
			return bestsockfd;

		struct sockaddr_storage bcaddr;

		if (!sock_getbroadcastaddr
		    ((struct sockaddr *) &sn, (struct sockaddr *) &bcaddr))
			continue;

		if (sock_cmpaddr((struct sockaddr *) &bcaddr, sa, 0))
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



int socklist_add_multicast(const char *addr, const char *port, int ac_proto)
{

	struct addrinfo hints;
	struct addrinfo *res, *res0;
	memset(&hints, 0, sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = PF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;

	int rc = getaddrinfo(addr, port, &hints, &res0);
	if (rc != 0) {
		cw_log(LOG_ERR, "Can't bind multicast address '%s': %s", addr,
		       gai_strerror(rc));
		return 0;
	}

	for (res = res0; res; res = res->ai_next) {
		struct sockaddr *sa = res->ai_addr;
		int sockfd = socket(res->ai_addr->sa_family, SOCK_DGRAM, 0);
		/* create socket */
		if (sockfd == -1) {
			cw_log(LOG_ERR, "Can't create multicast socket: %",
			       strerror(errno));
			continue;
		}


		/* bind address */
		if (bind(sockfd, sa, sock_addrlen(sa)) < 0) {
			close(sockfd);
			cw_log(LOG_ERR, "Can't bind multicast %s: %s", addr,
			       strerror(errno));
			continue;
		}

		/* use setsockopt() to request that the kernel joins a multicast group */
		void *opt;
		int optlen;
		if (res->ai_addr->sa_family == AF_INET) {

			struct ip_mreq mreq;
			memset(&mreq, 0, sizeof(mreq));
			struct sockaddr_in *sain = (struct sockaddr_in *) res->ai_addr;
			mreq.imr_multiaddr.s_addr = sain->sin_addr.s_addr;
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);
			opt = &mreq;
			optlen = sizeof(mreq);

			char sinin[100];
			sock_addrtostr((struct sockaddr *) sain, sinin, 100,1);

			if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, opt, optlen)
			    < 0) {
				close(sockfd);
				cw_log(LOG_ERR, "Can't add multicast membership %s: %s",
				       addr, strerror(errno));
				continue;
			}

		}
		if (res->ai_addr->sa_family == AF_INET6) {
			struct ipv6_mreq mreq;
			memset(&mreq, 0, sizeof(mreq));
			struct sockaddr_in6 *sain6 = (struct sockaddr_in6 *) res->ai_addr;
//                      mreq.ipv6mr_multiaddr.s_addr=sain->sin_addr.s_addr;
			memcpy(&mreq.ipv6mr_multiaddr.s6_addr, &sain6->sin6_addr.s6_addr,
			       sizeof(sain6->sin6_addr.s6_addr));
//                      int si  = sizeof(sain6->sin6_addr.s6_addr);

//                      int i = sain6->sin6_addr.s6_addr;                       
			mreq.ipv6mr_interface = 0;	//htonl(INADDR_ANY);
			opt = &mreq;
			optlen = sizeof(mreq);
			if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_JOIN_GROUP, opt, optlen)
			    < 0) {
				close(sockfd);
				cw_log(LOG_ERR, "Can't join multicast group %s: %s", addr,
				       strerror(errno));
				continue;
			}
		}

		int rfd = find_reply_socket(sa, 0);

		socklist[socklist_len].sockfd = sockfd;
//		socklist[socklist_len].reply_sockfd = rfd;
		socklist[socklist_len].type = SOCKLIST_BCASTMCAST_SOCKET;
		socklist[socklist_len].family = sa->sa_family;
		socklist[socklist_len].ac_proto = ac_proto;

		socklist_len++;

		cw_log(LOG_INFO, "Bound to multicast group: %s (fd=%i,r:%i)", addr,
		       sockfd, rfd);
	}

	freeaddrinfo(res0);
	return 1;
}

static int socklist_check_size()
{
	if (socklist_len>0 && (socklist_len % SOCKLIST_SIZE)==0){
		struct socklistelem *newsocklist;
		newsocklist = realloc(socklist, sizeof(struct socklistelem)*(socklist_len+SOCKLIST_SIZE));
		if (!newsocklist) {
			cw_log(LOG_ERR,"Can't increase socklist size, realoc failed");
			return 0;
		}
		socklist = newsocklist;
	}
	return 1;
}


int socklist_add_unicast(const char *addr, const char *port, int ac_proto)
{


	if (!socklist_check_size())
		return 0;
	

	struct addrinfo hints;
	struct addrinfo *res, *res0;
	memset(&hints, 0, sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = PF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;

	int rc = getaddrinfo(addr, port, &hints, &res0);
	if (rc != 0) {
		cw_log(LOG_ERR, "Can't bind unicast address '%s': %s", addr,
		       gai_strerror(rc));
		return 0;
	}

	for (res = res0; res; res = res->ai_next) {
		char ifname[64];
		struct sockaddr netmask;
		struct sockaddr broadcast;

		ifname[0] = 0;
		rc = sock_getifinfo(res->ai_addr, ifname, &broadcast, &netmask);
		if (!rc) {
			cw_log(LOG_ERR, "No interface found for %s, can't bind.", addr);
			continue;
		}

		/* Bind the control port */
		struct sockaddr *sa = res->ai_addr;
		int sockfd = socket(res->ai_addr->sa_family, SOCK_DGRAM, 0);
		/* create socket */
		if (sockfd == -1) {
			cw_log(LOG_ERR, "Can't create unicast socket: %s",
			       strerror(errno));
			continue;
		}

		/* bind address */
		if (bind(sockfd, sa, sock_addrlen(sa)) < 0) {
			close(sockfd);
			cw_log(LOG_ERR, "Can't bind unicast socket %s: %s", addr,
			       strerror(errno));
			continue;
		}


		socklist[socklist_len].sockfd = sockfd;
//		socklist[socklist_len].reply_sockfd = sockfd;
		socklist[socklist_len].family = sa->sa_family;
		socklist[socklist_len].type = SOCKLIST_UNICAST_SOCKET;
		socklist[socklist_len].ac_proto = ac_proto;


		



		if (res->ai_addr->sa_family == AF_INET) {
			memcpy(&socklist[socklist_len].netmask, &netmask,
			       sock_addrlen(&netmask));
			memcpy(&socklist[socklist_len].addr, res->ai_addr,
			       sock_addrlen(res->ai_addr));
			cw_log(LOG_INFO,
			       "Bound to: %s:%s (%i) on interface %s, netmask %s", addr,
			       port, sockfd, ifname, sock_addr2str(&netmask));
		} else {
			cw_log(LOG_INFO, "Bound to: [%s]:%s (%i) on interface %s", addr,
			       port, sockfd, ifname);
		}

		
		/* Bind the data port */
		sa = res->ai_addr;

		/* XXX data port is currently hard coded */
		sock_setport(sa,5247);

		sockfd = socket(res->ai_addr->sa_family, SOCK_DGRAM, 0);
		/* create socket */
		if (sockfd == -1) {
			cw_log(LOG_ERR, "Can't create unicast data socket: %s",
			       strerror(errno));
			continue;
		}

		/* bind address */
		if (bind(sockfd, sa, sock_addrlen(sa)) < 0) {
			close(sockfd);
			cw_log(LOG_ERR, "Can't bind unicast data socket %s: %s", addr,
			       strerror(errno));
			continue;
		}

		cw_log(LOG_INFO, "Bound to data port: [%s]:%s (%i) on interface %s", addr,
			       "5247", sockfd, ifname);

		socklist[socklist_len].data_sockfd = sockfd;

		socklist_len++;

	}

	freeaddrinfo(res0);
	return 1;
}

int socklist_add_broadcast(const char *addr, const char *port, int ac_proto)
{
	if (!socklist_check_size())
		return 0;

	struct addrinfo hints;
	struct addrinfo *res, *res0;
	memset(&hints, 0, sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = PF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;


	int rc = getaddrinfo(addr, port, &hints, &res0);
	if (rc != 0) {
		cw_log(LOG_ERR, "Can't bind broadcast address '%s': %s", addr,
		       gai_strerror(rc));
		return 0;
	}

	int sockfd;
	for (res = res0; res; res = res->ai_next) {

		struct sockaddr *sa = res->ai_addr;
		sockfd = socket(res->ai_addr->sa_family, SOCK_DGRAM, 0);

		/* create socket */
		if (sockfd == -1) {
			cw_log(LOG_ERR, "Can't create broadcast socket: %",
			       strerror(errno));
			continue;
		}
#ifdef IP_BINDANY
		struct sockaddr_in *sain = (struct sockaddr_in *) sa;
		if (sain->sin_addr.s_addr == INADDR_BROADCAST) {
			int opt = 1;
			if (setsockopt(sockfd, IPPROTO_IP, IP_BINDANY, &opt, sizeof(opt))) {
				cw_log(LOG_ERR, "Can't set sockopt IP_BIND_ANY: %s",
				       strerror(errno));
				continue;
			};
		}
#endif

		/* bind address */
		if (bind(sockfd, sa, sock_addrlen(sa)) < 0) {
			close(sockfd);
			sockfd = -1;
			cw_log(LOG_ERR, "Can't bind  broadcast %s: %s", addr,
			       strerror(errno));
			continue;
		}


		int rfd = find_reply_socket(sa, 1);

		socklist[socklist_len].sockfd = sockfd;
//		socklist[socklist_len].reply_sockfd = rfd;
		socklist[socklist_len].type = SOCKLIST_BCASTMCAST_SOCKET;
		socklist[socklist_len].family = sa->sa_family;
		socklist[socklist_len].ac_proto = ac_proto;

		memcpy(&socklist[socklist_len].addr, res->ai_addr,
		       sock_addrlen(res->ai_addr));

//              printf ("AC INIT PROTO : %d, i %i\n",ac_proto,socklist_len);
//              printf ("sock proto %d\n",socklist[socklist_len].ac_proto);
		socklist_len++;

		cw_log(LOG_INFO, "Bound to broadcast: %s:%s (%i,R:%i,I:%d)", addr, port,
		       sockfd, rfd, socklist_len - 1);
	}

	freeaddrinfo(res0);
	return 1;
}
