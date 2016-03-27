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

#ifndef __SOCKLIST_H
#define __SOCKLIST_H

#define SOCKLIST_SIZE 32

enum {
	SOCKLIST_UNICAST_SOCKET,
	SOCKLIST_BCASTMCAST_SOCKET
};

struct socklistelem{
	int sockfd;
	int data_sockfd;

	int type;
	int family;
	int wtpcount;
	int ac_proto;
	struct sockaddr netmask;
	struct sockaddr addr;
};


extern int socklist_add_multicast(const char * addr, const char * port, int ac_proto);
extern int socklist_add_unicast(const char *addr, const char * port, int ac_proto);
extern int socklist_add_broadcast(const char *addr, const char * port,int ac_proto);
extern int socklist_init();
extern void socklist_destroy();
extern struct socklistelem * socklist;
extern int socklist_len;
extern void socklist_lock();
extern void socklist_unlock();
void socklist_add_connection(int index);
void socklist_del_connection(int index);
extern int socklist_find_reply_socket(struct sockaddr *sa, int port);


#endif
