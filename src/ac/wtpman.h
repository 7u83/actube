#ifndef __WTPMAN_H
#define __WTPMAN_H

#include <sys/socket.h>
#include <semaphore.h>
#include <pthread.h>

#include "cw/capwap.h"
#include "cw/fragman.h"
#include "cw/timer.h"

#define WTPMAN_QSIZE 1024

struct wtpman{
	pthread_t thread;

	struct conn * conn;


	/* wtp data */
	uint8_t * wtp_name;
	uint8_t * wtp_location;
	uint8_t * session_id;
	uint8_t wtp_mactype;

	int socklistindex;
	int connected;

	int dtlsmode;

	cw_timer_t echointerval_timer; 


};

/*
struct wtp{
		

};


//extern void wtpman_run(void *arg);
*/

struct wtpman * wtpman_create();

struct wtpman * wtpman_create(int socklistindex,struct sockaddr * srcaddr, int dtlsmode);

extern void wtpman_addpacket(struct wtpman * wtpman,uint8_t *packet,int len);
extern void wtpman_destroy(struct wtpman * wtpman);
extern void wtpman_start(struct wtpman * wtpman,int dtlsmode);




#endif
