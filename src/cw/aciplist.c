/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
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

/**
 * @file
 * @brief Implements some aciplist functions
 */

#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "aciplist.h"
#include "avltree.h"
#include "sock.h"


static int acip_cmp(const void *x1, const void *x2)
{
	struct cw_acip *ip1 = (struct cw_acip *) x1;
	struct cw_acip *ip2 = (struct cw_acip *) x2;
		
	int r = ip1->index - ip2->index;
	if (r!=0)
		return r;


	if (ip1->ip.ss_family != ip2->ip.ss_family) {
		if (ip1->ip.ss_family == AF_INET)
			return -1;
		else
			return 1;
	}

	if (ip1->ip.ss_family == AF_INET) {
		struct sockaddr_in *s2 = (struct sockaddr_in *) &ip2->ip;
		struct sockaddr_in *s1 = (struct sockaddr_in *) &ip1->ip;

		return memcmp(&s1->sin_addr, &s2->sin_addr,
			      sock_addrlen((struct sockaddr *) s1));
	}

	if (ip1->ip.ss_family == AF_INET6) {
		struct sockaddr_in6 *s2 = (struct sockaddr_in6 *) &ip2->ip;
		struct sockaddr_in6 *s1 = (struct sockaddr_in6 *) &ip1->ip;

		return memcmp(&s1->sin6_addr.s6_addr, &s2->sin6_addr.s6_addr,
			      sock_addrlen((struct sockaddr *) s1));
	}

	return -1;
}

static void acip_del(void *d)
{
	free(d);
}

/**
 * Create a cw_aciplist_t object
 * @return The created aciplist or NULL if an error has occured.
 */
cw_aciplist_t cw_aciplist_create()
{
	return  avltree_create(acip_cmp, acip_del);
}

