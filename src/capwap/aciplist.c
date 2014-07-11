#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "acinfo.h"
#include "avltree.h"
#include "sock.h"


static int acip_cmp(const void *x1, const void *x2)
{
	struct acip * ip1 = (struct acip*)x1;
	struct acip * ip2 = (struct acip*)x2;

	if (ip1->ip.ss_family != ip2->ip.ss_family){
		if (ip1->ip.ss_family == AF_INET)
			return -1;
		else
			return 1;
	}

	if (ip1->ip.ss_family == AF_INET){
		struct sockaddr_in * s2=(struct sockaddr_in*)&ip2->ip;
		struct sockaddr_in * s1=(struct sockaddr_in*)&ip1->ip;

		return memcmp(&s1->sin_addr,&s2->sin_addr,sock_addrlen((struct sockaddr*)s1));
	}

	if (ip1->ip.ss_family == AF_INET6){
		struct sockaddr_in6 * s2=(struct sockaddr_in6*)&ip2->ip;
		struct sockaddr_in6 * s1=(struct sockaddr_in6*)&ip1->ip;

		return memcmp(&s1->sin6_addr.s6_addr,&s2->sin6_addr.s6_addr,sock_addrlen((struct sockaddr*)s1));
	}

	return -1;
}

static void acip_del(void*d)
{
	free(d);
}

ACIPLIST * aciplist_create()
{
	struct avltree *t = avltree_create(acip_cmp,acip_del);
	return t;
}



