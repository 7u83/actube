#ifndef __WTPMAN_H
#define __WTPMAN_H

#include <sys/socket.h>
#include <semaphore.h>
#include <pthread.h>

#include "cw/capwap.h"
#include "cw/fragman.h"
#include "cw/timer.h"
#include "cw/cfg.h"
#include "cw/capwap.h"

#define WTPMAN_QSIZE 1024

struct wtpman {
	pthread_t thread;

	struct cw_Conn *conn;


	/* wtp data */
	uint8_t *wtp_name;
	uint8_t *wtp_location;
	uint8_t *session_id;
	uint8_t wtp_mactype;

	int socklistindex;
	int connected;

	int dtlsmode;

	cw_timer_t echointerval_timer;

	cw_Cfg_t * global_cfg; /**< contains the cfg of AC, 
				 visible to all wtpman threads.  
				 The global cfg was initally read
				 from a .ckv file on startup.
				 */

	cw_Cfg_t * wtp_cfg;

	

};

/*
struct wtp{
		

};


//extern void wtpman_run(void *arg);
*/

struct wtpman *wtpman_create();

struct wtpman *wtpman_create(int socklistindex, struct sockaddr *srcaddr,
			     int dtlsmode, cw_Cfg_t * global_cfg);

extern void wtpman_addpacket(struct wtpman *wtpman, uint8_t * packet, int len);
extern void wtpman_destroy(struct wtpman *wtpman);
extern void wtpman_start(struct wtpman *wtpman, int dtlsmode);




#endif
