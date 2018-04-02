#include <errno.h>
#include <time.h>

#include "conn.h"

int conn_q_wait_packet(struct conn * conn, int seconds)
{

	struct timespec timespec;
	clock_gettime(CLOCK_REALTIME,&timespec);
	timespec.tv_sec+=seconds;


	/* wait one second to get a packet */
	if (sem_timedwait(&conn->q_sem,&timespec)==-1){
		return EAGAIN;
	};
	sem_post(&conn->q_sem);
	return 0;
}
