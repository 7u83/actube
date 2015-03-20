#include "conn.h"
#include "capwap.h"

void conn_prepare_request(struct conn * conn, int type)
{
	struct cwmsg * cwmsg = &conn->req_msg;
	uint8_t * buffer = conn->req_buffer;
	cwmsg_init(cwmsg,buffer,type,conn_get_next_seqnum(conn),0);
	cwmsg->capwap_mode=conn->capwap_mode;
}
