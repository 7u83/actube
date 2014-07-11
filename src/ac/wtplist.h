#ifndef __WTPLIST_H
#define __WPTLIST_H

#include <sys/socket.h>

#include "wtpman.h"

extern int wtplist_init();
extern void wtplist_destroy();

extern struct wtpman * wtplist_get(const struct sockaddr * addr);
extern struct wtpman * wtplist_add(struct wtpman * wtpman);
extern void wtplist_remove(struct wtpman * wtpman);
extern void wtplist_lock();
extern void wtplist_unlock();


#endif
