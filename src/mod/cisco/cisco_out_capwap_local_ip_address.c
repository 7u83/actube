#include "cw/conn.h"

#include "mod_cisco.h"
#include "cw/cw.h"
#include "cw/dbg.h"


int cisco_out_capwap_local_ip_address(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	const char * ipstr;
	int rc;
	cw_Val_t ip;
	ipstr = cw_cfg_get_l(params->cfg_list,eh->key,NULL);
	if (ipstr==NULL){
		return 0;
	}
	CW_TYPE_IPADDRESS->from_str(&ip,ipstr);
	rc= cw_put_local_ip_address(dst,eh->id,
			CAPWAP_ELEM_WTP_IPV4_IP_ADDRESS,
			CAPWAP_ELEM_WTP_IPV6_IP_ADDRESS,
			ip.type->data(&ip),ip.type->len(&ip));
	ip.type->del(&ip);
	return rc;
}
