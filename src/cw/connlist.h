#ifndef __CONLIST_H
#define __CONLIST_H


#include "conn.h"
#include "mavl.h"

struct connlist {
/*	struct conn ** connlist; */

	struct mavl *by_addr;
	struct mavl *by_session_id;


	int len;
	pthread_mutex_t connlist_mutex;
};


struct connlist *connlist_create(int len);
void connlist_lock(struct connlist *cl);
void connlist_unlock(struct connlist *cl);
void conlist_destroy(struct connlist *cl);
struct conn *connlist_get(struct connlist *cl, const struct sockaddr *addr);
struct conn *connlist_add(struct connlist *cl, struct conn *conn);
void connlist_remove(struct connlist *cl, struct conn *conn);
void connlist_destroy(struct connlist *cl);

struct conn * connlist_get_by_session_id(struct connlist *cl,  struct conn * conn);
struct conn * connlist_add_by_session_id(struct connlist * cl, struct conn * conn);

#endif
