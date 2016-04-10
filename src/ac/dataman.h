#ifndef __DATAMAN_H
#define __DATAMAN_H

#include <pthread.h>

#include "cw/netconn.h"

#include "wtpman.h"

struct dataman {
	struct netconn *nc;
	pthread_t thread;
	struct wtpman * wtpman;


};

extern int dataman_list_init();
struct dataman *dataman_list_add(struct dataman *dm);
extern struct dataman *dataman_list_get(int sock, struct sockaddr *addr);


extern struct dataman *dataman_create(int sock, struct sockaddr *addr);
extern void dataman_start(struct dataman * dm);

extern pthread_mutex_t dataman_list_mutex;

#define dataman_list_lock() pthread_mutex_lock(&dataman_list_mutex)
#define dataman_list_unlock() pthread_mutex_unlock(&dataman_list_mutex)

#define dataman_add_packet(dm,data,len) (netconn_q_add_packet(dm->nc,data,len))

#endif
