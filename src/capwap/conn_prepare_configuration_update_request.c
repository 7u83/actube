

#include "conn.h"
#include "capwap.h"


void conn_prepare_configuration_update_request(struct conn * conn)
{
	struct cwmsg * cwmsg = &conn->req_msg;
	uint8_t * buffer = conn->req_buffer;
	cwmsg_init(cwmsg,buffer,CW_MSG_CONFIGURATION_UPDATE_REQUEST,conn_get_next_seqnum(conn),0);
}
