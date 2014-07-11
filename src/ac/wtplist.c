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


#define MAX_WTPS 200

#include <stdio.h>

#include "wtplist.h"
#include "conn.h"
#include "sock.h"


#include "conf.h"
#include "cw_log.h"

/*
static struct wtpman ** wtplist = 0;
static pthread_mutex_t wtplist_mutex;

static int max_wtp_connections;
*/


static struct connlist * connlist;


int wtplist_init()
{

	connlist  = connlist_create(0);
	if (!connlist)
		return 0;

	return 1;
}

void wtplist_lock()
{
	connlist_lock(connlist);

}

void wtplist_unlock()
{
	connlist_unlock(connlist);
}


void wtplist_destroy()
{
	connlist_destroy(connlist);

/*	if (wtplist)
		free (wtplist);
	pthread_mutex_destroy(&wtplist_mutex);
*/	
}


struct wtpman * wtplist_get(const struct sockaddr * addr)
{

	struct conn * conn = connlist_get(connlist,addr);
	if (!conn)
		return 0;
	return conn->data;

/*
	int i;
	for (i=0; i<max_wtp_connections; i++){
		if (!wtplist[i])
			continue;

		if ( sock_cmpaddr(&wtplist[i]->conn->addr,addr,1))
			continue;
		struct wtpman * wtpman = wtplist[i];
		return wtpman;

	}
	return NULL;
*/	
}


struct wtpman * wtplist_add(struct wtpman * wtpman)
{
	wtpman->conn->data=wtpman;	
	return connlist_add(connlist,wtpman->conn);


/*
	int i;
	for (i=0; i<max_wtp_connections; i++){
		if (!wtplist[i]){
			wtplist[i]=wtpman;
			return wtpman;
		}
	}
	return NULL;
*/

}


void wtplist_remove(struct wtpman * wtpman)
{
	connlist_remove(connlist,wtpman->conn);
	return;

/*
	printf("Remove wtpman %p\n",wtpman);
	int i;
	for (i=0; i<max_wtp_connections; i++){
		if (wtplist[i]==wtpman){
			printf("wtpman removed\n");
			wtplist[i]=0;
			break;
		}
	}
*/

}
	


