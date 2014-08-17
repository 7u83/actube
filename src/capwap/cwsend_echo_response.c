#include "capwap.h"
#include "conn.h"
#include "cwmsg.h"

int cwsend_echo_response(struct conn * conn,int seqnum,struct radioinfo * radioinfo) //,struct wtpinfo * wtpinfo)
{
	//uint8_t buffer[CWMSG_MAX_SIZE];
	struct cwmsg * cwmsg = &conn->resp_msg;	
	cwmsg_init(cwmsg,conn->resp_buffer,CWMSG_ECHO_RESPONSE,seqnum,radioinfo);

	conn_send_response(conn,cwmsg,seqnum);
	return 1;

	//cwmsg_init(&cwmsg,buffer,CWMSG_ECHO_REQUEST,conn_get_next_seqnum(conn),radioinfo);
	//	return conn_send_cwmsg(conn,&cwmsg);

}
