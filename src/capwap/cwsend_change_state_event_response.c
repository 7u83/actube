
#include "cwmsg.h"
#include "capwap.h"
#include "cw_log.h"
#include "sock.h"

void cwsend_change_state_event_response(struct conn * conn,int seqnum, struct radioinfo * radioinfo)
{
	cw_dbg(DBG_CW_MSG,"Sending change state response to %s, seq = %d",sock_addr2str(&conn->addr),seqnum);

	struct cwmsg * cwmsg = &conn->resp_msg;	
	cwmsg_init(cwmsg,conn->resp_buffer,CWMSG_CHANGE_STATE_EVENT_RESPONSE,seqnum,NULL);

	cwmsg_addelem_result_code(cwmsg,0);
	cwmsg_addelem_radio_operational_state(cwmsg,radioinfo);
	
	conn_send_response(conn,cwmsg,seqnum);
}

