
#include <time.h>

#include "conn.h"


uint8_t * conn_q_get_packet(struct conn * conn)
{
	struct timespec timespec;
	clock_gettime(CLOCK_REALTIME,&timespec);
	timespec.tv_sec++;


//	sem_wait(&conn->q_sem);
	if (sem_timedwait(&conn->q_sem,&timespec)==-1){
		return NULL;
	};
	int qrpos = conn->qrpos+1;
	if (qrpos==conn->qsize)
		qrpos=0;
	conn->qrpos=qrpos;
	return conn->q[qrpos];
/*	
	uint8_t * packet = conn->q[qrpos]+4;
	int len = *( (uint32_t*)(conn->q[qrpos]));

	conn_process_packet(conn->conn,packet,len);
//	free(conn->q[qrpos]);
*/

}

