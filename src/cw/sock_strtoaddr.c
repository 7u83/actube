#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "sock.h"
/**
 * Convert a string to an sockaddr struct.
 * The string can contain an ipv4 or ipv6 address, including a port number.
 * @param s address string
 * @param saout output buffer
 * @return 1 on success,  otherwise no success
 */
int sock_strtoaddr(const char * s,struct sockaddr * saout){

	char *ips,*ps;
	struct in_addr ia;
	int port;

#ifdef WITH_IPV6
	struct in6_addr ia6;
#endif
	int rc;

	/* copy the string */
	ips = malloc(strlen(s)+1);
	strcpy(ips,s);

	/* search for a collon to separate the port */
	ps = strchr(ips,':');
	if (ps != NULL){
		*ps='\0';
		ps++;
	}
	else
		ps="0";

	/* try to parse ip4 address */
	rc = inet_pton(AF_INET,ips,&ia);


	if (rc==1)
	{
		port = atoi(ps);
		if (port > 65535)
			rc=0;
	}



	if (rc==1){
		/* it's an ipv4 address */
		struct sockaddr_in * sa;
		sa = (struct sockaddr_in*)saout;
		memset(sa,0,sizeof(struct sockaddr_in));
#ifdef HAVE_SIN_LEN
		sa->sin_len=sizeof(struct sockaddr_in);
#endif
		sa->sin_family = AF_INET;
		sa->sin_addr=ia;
		sa->sin_port=htons(port);

	}

#ifdef WITH_IPV6
	if (rc==0){
		strcpy(ips,s);

		ps = strchr(ips,'.');
		if (ps != NULL){
			*ps='\0';
			ps++;
		}
		else
			ps="0";


		rc = inet_pton(AF_INET6,s,&ia6);


		if (rc==1){
			/* it's an ipv6 address */
			struct sockaddr_in6 * sa;
			sa = (struct sockaddr_in6*)saout;
			memset(sa,0,sizeof(struct sockaddr_in6));

#ifdef HAVE_SIN_LEN
			sa->sin6_len=sizeof(struct sockaddr_in6);
#endif
			sa->sin6_family = AF_INET6;
			sa->sin6_addr=ia6;
			sa->sin6_port=htons(atoi(ps));


		}

	}
#endif
	if (rc!=1){
		if (rc!=-1)
			errno=EINVAL;
	}

	free (ips);
	return rc;

}

