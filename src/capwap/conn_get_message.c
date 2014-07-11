

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "conn.h"
#include "cwrmsg.h"




static int pmessage(void *w, struct cwrmsg * cwrmsg)
{
	uint8_t * buffer;
	buffer = malloc( sizeof (struct cwrmsg) + cwrmsg->msgelems_len);
	memcpy(buffer,cwrmsg,sizeof(struct cwrmsg));
	memcpy(buffer+sizeof(struct cwrmsg),cwrmsg->msgelems,cwrmsg->msgelems_len);
	((struct cwrmsg*)(buffer))->msgelems=buffer+sizeof(struct cwrmsg);
	*((void**)w) = (void*)buffer;
	return 0;
}




struct cwrmsg * conn_get_message(struct conn * conn)
{
	struct cwrmsg * cwrmsg=0;
//	conn->process_message=pmessage;
//	conn->pmsgarg=&cwrmsg;

	uint8_t buf[2048];
	int len=2048;
//	int flags=0;


	int n;
//	do {
//		n = conn->recv_packet(conn,(char*)buf,len);
		n = conn->read(conn,buf,len);
//
//		printf("GETMPACK %i\n",n);
		if (n>0) 
			conn_process_packet(conn,buf,n,pmessage,&cwrmsg);

//	} while (cwrmsg==0);	

	return cwrmsg;
}

