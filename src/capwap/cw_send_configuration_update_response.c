

#include "capwap.h"

int cw_send_configuration_update_response(struct conn * conn,int seqnum,struct radioinfo * radioinfo)
{
	struct cwmsg * cwmsg = &conn->resp_msg;	
	cwmsg_init(cwmsg,conn->resp_buffer,CW_MSG_CONFIGURATION_UPDATE_RESPONSE,seqnum,radioinfo);

	cwmsg_addelem_result_code(cwmsg,0);

	conn_send_response(conn,cwmsg,seqnum);
	return 1;
}
