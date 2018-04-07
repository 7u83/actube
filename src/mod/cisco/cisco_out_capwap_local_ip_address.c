#include "mod_cisco.h"
#include "cw/cw.h"

int cisco_out_capwap_local_ip_address(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	cw_KTV_t * ip;
	ip = cw_ktv_get(params->conn->local_cfg,eh->key,CW_TYPE_IPADDRESS);
	if (ip==NULL){
		return 0;
	}
	return cw_put_local_ip_address(dst,eh->id,
			CAPWAP_ELEM_WTP_IPV4_IP_ADDRESS,
			CAPWAP_ELEM_WTP_IPV6_IP_ADDRESS,
			ip->type->data(ip),ip->type->len(ip));
}
