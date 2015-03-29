
#include "capwap.h"
#include "radioinfo.h"



void cw_prepare_change_state_event_request(struct conn * conn, struct radioinfo * radioinfo, struct wtpinfo *wtpinfo)
{
	struct cwmsg * cwmsg = &conn->req_msg;
	uint8_t * buffer = conn->req_buffer;

	cwmsg_init(cwmsg,buffer,CW_MSG_CHANGE_STATE_EVENT_REQUEST,conn_get_next_seqnum(conn),radioinfo);
	cwmsg->capwap_mode=conn->capwap_mode;

	cwmsg_addelem_radio_operational_state(cwmsg,&wtpinfo->radioinfo[0]);
	cwmsg_addelem_radio_operational_state(cwmsg,&wtpinfo->radioinfo[1]);

	cwmsg_addelem_result_code(cwmsg,0);
}	
