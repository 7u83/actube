
#include <string.h>


#include "capwap.h"
#include "conn.h"

void cwsend_join_response(struct conn * conn,int seqnum, int rc, struct radioinfo * radioinfo, struct ac_info * acinfo, struct wtpinfo * wtpinfo)
{
	struct cwmsg * cwmsg = &conn->swm;	
	cwmsg_init(cwmsg,conn->buffer,CWMSG_JOIN_RESPONSE,seqnum,radioinfo);

	cwmsg_addelem_result_code(cwmsg,rc);
	cwmsg_addelem_ac_descriptor(cwmsg,acinfo);

	cwmsg_addelem(cwmsg,CWMSGELEM_AC_NAME,acinfo->ac_name,strlen((char*)acinfo->ac_name));
	cwmsg_addelem(cwmsg,CWMSGELEM_ECN_SUPPORT,&acinfo->ecn_support,sizeof(uint8_t));
	cwmsg_addelem_ctrl_ip_addrs(cwmsg,acinfo);
	cwmsg_addelem_cw_local_ip_addr(cwmsg,conn);
	
	conn_send_response(conn,cwmsg,seqnum);

}



