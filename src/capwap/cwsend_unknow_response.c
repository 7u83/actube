#include "capwap.h"
#include "conn.h"


void cwsend_unknown_response(struct conn * conn,int seqnum, int unknown_request)
{

	struct cwmsg * cwmsg = &conn->resp_msg;	
	cwmsg_init(cwmsg,conn->resp_buffer,unknown_request+1,seqnum,NULL);
	cwmsg_addelem_result_code(cwmsg,19);
	conn_send_response(conn,cwmsg,seqnum);
}

