

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

	uint8_t buf[2048];
	int len=2048;

	int n = conn->read(conn,buf,len);
	if (n>0) 
		conn_process_packet(conn,buf,n,pmessage,&cwrmsg);

	return cwrmsg;
}

