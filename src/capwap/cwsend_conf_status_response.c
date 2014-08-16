

#include "capwap.h"
#include "conn.h"

void cwmsg_addelem_capwap_timers(struct cwmsg *msg,int discovery,int echo_request)
{
	uint8_t timers[2];
	timers[0]=discovery;
	timers[1]=echo_request;
	cwmsg_addelem(msg,CWMSGELEM_CAPWAP_TIMERS,timers,sizeof(timers));
}

void cwsend_conf_status_response(struct conn * conn,int seqnum, int rc, struct radioinfo * radioinfo, struct ac_info * acinfo, struct wtpinfo * wtpinfo)
{
	struct cwmsg * cwmsg = &conn->swm;	
	cwmsg_init(cwmsg,conn->buffer,CWMSG_CONFIGURATION_STATUS_RESPONSE,seqnum,NULL);

	cwmsg_addelem_capwap_timers(cwmsg,120,15);
/*
	cwmsg_addelem_result_code(cwmsg,rc);
	cwmsg_addelem_ac_descriptor(cwmsg,acinfo);

	cwmsg_addelem(cwmsg,CWMSGELEM_AC_NAME,acinfo->ac_name,strlen((char*)acinfo->ac_name));
	cwmsg_addelem(cwmsg,CWMSGELEM_ECN_SUPPORT,&acinfo->ecn_support,sizeof(uint8_t));
	cwmsg_addelem_ctrl_ip_addrs(cwmsg,acinfo);
	cwmsg_addelem_cw_local_ip_addr(cwmsg,conn);
*/	
	conn_send_response(conn,cwmsg,seqnum);

}



