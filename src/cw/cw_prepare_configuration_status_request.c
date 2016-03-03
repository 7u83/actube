
#include "capwap.h"
#include "lwapp_cisco.h"
#include "capwap_cisco.h"


#include "conn.h"
#include "wtpinfo.h"

void cw_prepare_configuration_status_request(struct conn * conn, struct radioinfo * radioinfo, struct wtpinfo *wtpinfo)
{
	struct cwmsg * cwmsg = &conn->req_msg;
	uint8_t * buffer = conn->req_buffer;

	cwmsg_init(cwmsg,buffer,CW_MSG_CONFIGURATION_STATUS_REQUEST,conn_get_next_seqnum(conn),radioinfo);
	cwmsg->capwap_mode=conn->capwap_mode;
	
	cwmsg_addelem_ac_name(cwmsg,(uint8_t *)"AC-iMaxi");	


	cwmsg_addelem_cisco_ap_regulatory_domain(cwmsg,&wtpinfo->radioinfo[0]);
	cwmsg_addelem_cisco_ap_regulatory_domain(cwmsg,&wtpinfo->radioinfo[1]);
	
	cwmsg_addelem_cisco_wtp_radio_cfg(cwmsg,&wtpinfo->radioinfo[0]);
	cwmsg_addelem_cisco_wtp_radio_cfg(cwmsg,&wtpinfo->radioinfo[1]);

/*
	uint8_t mtu[2048];
	int l = lw_put_cisco_path_mtu(mtu,1485,1701);

	printf("Len = %d\n",l);

	cwmsg_addelem_vendor_specific_payload(cwmsg,LW_VENDOR_CISCO,
		LW_ELEM_VENDOR_SPECIFIC,mtu,l);

*/
}
