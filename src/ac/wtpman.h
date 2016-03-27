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

	/* packet queue */
/*	uint8_t * q[WTPMAN_QSIZE];
	int qrpos;
	int qwpos;
	sem_t q_sem;
*/
	struct conn * conn;


	/* wtp data */
	uint8_t * wtp_name;
	uint8_t * wtp_location;
	uint8_t * session_id;
	uint8_t wtp_mactype;

	//struct wtpinfo wtpinfo;


	int socklistindex;
	int connected;


	cw_timer_t echointerval_timer; 


};

struct wtp{
		

};


//extern void wtpman_run(void *arg);
extern struct wtpman * wtpman_create();

extern struct wtpman * wtpman_create(int socklistindex,struct sockaddr * srcaddr);

extern void wtpman_addpacket(struct wtpman * wtpman,uint8_t *packet,int len);
extern void wtpman_destroy(struct wtpman * wtpman);
extern void wtpman_start(struct wtpman * wtpman,int dtlsmode);




#endif
