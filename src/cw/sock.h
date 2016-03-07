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


/**
 *@file
 *@breif
 *@defgroup SOCK SOCK
 *@{
 */

#ifndef __SOCK_H
#define __SOCK_H


#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>


#ifdef __FreeBSD__
#define HAVE_SIN_LEN
#define HAVE_SIN6_LEN
#endif



extern int sock_create(const struct sockaddr *sa);
extern int sock_mwait(int *socklist, int socklistlen, fd_set * fset);
extern void sock_copyaddr(struct sockaddr_storage *dst, const struct sockaddr *src);
extern int sock_cmpaddr(const struct sockaddr *addr1, const struct sockaddr *addr2,
			int cmpport);
extern int sock_addrlen(const struct sockaddr *sa);
extern int sock_receive(int sock, void *buf, size_t len, int flags,
			struct sockaddr *srcaddr, socklen_t * addrlen);
extern int sock_mreceive(int *socklist, int socklistlen,
			 void (*callback) (int *socklist, int index, struct sockaddr *,
					   uint8_t * buf, int len));
extern int sock_getifaddr(const char *ifname, int family, int type, struct sockaddr *sa);
extern int sock_getifhwaddr(const char *ifname, uint8_t * hwaddr, uint8_t * addrlen);
extern char *sock_hwaddrtostr(const uint8_t * haddr, int len, char *dst,
			      const char *separator);
//extern char *sock_addrtostr(const struct sockaddr *sa, char *s, size_t maxlen);
extern int sock_strtoaddr(const char *s, struct sockaddr *saout);
extern int sock_set_recvtimeout(int sock, int seconds);

extern int sock_getport(struct sockaddr *addr);
extern int sock_setport(struct sockaddr *addr, int port);
extern int sock_getbroadcastaddr(const struct sockaddr *addr,
				 struct sockaddr *broadcastaddr);
extern void sock_addrinit(struct sockaddr *addr, int type);
extern int sock_set_dontfrag(int sock, int val);


extern char *sock_get_primary_if(int family);
//extern char * sock_getifinfo(const struct sockaddr *sa,struct ifaddrs *result);
extern int sock_getifinfo(const struct sockaddr *addr, char *ifname,
			  struct sockaddr *broadcast, struct sockaddr *netmask);



char *sock_addrtostr(const struct sockaddr *sa, char *s, size_t maxlen, int addport);


/**
 * Convert a sockaddr structure to a human readable string
 * @param s sockaddr
 * @return A char* pointer to converted address
 *
 * The returned string is pushed on the stack (unnamed variable), so this macro should
 * only be used as function argument.
 *
 * Example:
 * \code
   struct sockaddr sa;
   // Initialize sa ...
   sprintf("Adress: %s",sock_addr2str(&sa));
   \endcode 
 */
#define sock_addr2str(s) ( sock_addrtostr( (struct sockaddr*)s, (char[64]){0}, 64, 0 ) )

/**
 * Same as #sock_addr2str, but also the port number is appended to the result
 * @see #sock_addr2str
 */
#define sock_addr2str_p(s) ( sock_addrtostr( (struct sockaddr*)s, (char[64]){0}, 64,1 ) )



#define sock_hwaddr2str(s,l) ( sock_hwaddrtostr( s,l, (char[64]){0}, ":" ) )
#define sock_hwaddr2idstr(s,l) ( sock_hwaddrtostr( s,l, (char[64]){0}, "" ) )
//#define sock_addrtostr(s,str,n) sock_addrtostr(s,str,n,1)

#define sock_addrfamily(addr) ( ((struct sockaddr_storage*)(addr))->ss_family )

#endif				/* __SOCK_H */

/**@}*/
