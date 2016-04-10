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

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>



#include <stdio.h>

#include "conn.h"
#include "connlist.h"

#include "sock.h"





static int cmp_by_addr(const void * d1,const void *d2)
{
	struct conn * c1=(struct conn *) d1;
	struct conn * c2=(struct conn *) d2;

	return sock_cmpaddr((struct sockaddr*)&c1->addr,(struct sockaddr*)&c2->addr,1);
}

static int cmp_by_session_id(const void *d1, const void *d2)
{
	struct conn * c1=(struct conn *) d1;
	struct conn * c2=(struct conn *) d2;
	return memcmp(c1->session_id,c2->session_id,16);	
}


struct connlist * connlist_create(int len)
{

	struct connlist * cl = malloc(sizeof(struct connlist));
	if (!cl)
		return 0;


	cl->by_addr = mavl_create(cmp_by_addr,0);

	if (!cl->by_addr){
		free(cl);
		return 0;
	}

	cl->by_session_id = mavl_create(cmp_by_session_id,0);


	if (pthread_mutex_init(&cl->connlist_mutex,NULL)){
		mavl_destroy(cl->by_addr);
		free(cl);
		return 0;
	};

	cl->len=len;

	return cl;
}

void connlist_lock(struct connlist * cl)
{
	pthread_mutex_lock(&cl->connlist_mutex);
}

void connlist_unlock(struct connlist * cl)
{
	pthread_mutex_unlock(&cl->connlist_mutex);
}


void connlist_destroy(struct connlist * cl)
{
	if (!cl)
		return;

	if (cl->by_addr)
		mavl_destroy(cl->by_addr);
	pthread_mutex_destroy(&cl->connlist_mutex);
	free(cl);
	
}


struct conn * connlist_get(struct connlist * cl, const struct sockaddr * addr)
{
	struct conn search;
	sock_copyaddr(&search.addr,addr);
	return mavl_get(cl->by_addr,&search);
}


struct conn * connlist_add(struct connlist * cl, struct conn * conn)
{
	if ( cl->len!=0)
		if (cl->by_addr->count>=cl->len)
			return NULL;
	conn->connlist=cl;
	return mavl_add(cl->by_addr,conn);
}

struct conn * connlist_get_by_session_id(struct connlist *cl, struct conn * conn)
{
	return mavl_get(cl->by_session_id,conn);
}

struct conn * connlist_add_by_session_id(struct connlist * cl, struct conn * conn)
{
	return mavl_add(cl->by_session_id,conn);
}

void connlist_remove(struct connlist *cl,struct conn * conn)
{
	mavl_del(cl->by_session_id,conn);
	mavl_del(cl->by_addr,conn);
}
	


