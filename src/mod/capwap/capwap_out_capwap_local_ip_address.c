#include "cw/conn.h"
#include "mod_capwap.h"
#include "cw/cw.h"
#include "cw/dbg.h"

int capwap_out_capwap_local_ip_address(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	const char * ipstr;
	cw_Val_t ip;
	memset(&ip,0,sizeof(cw_Val_t));
	ip.type=CW_TYPE_IPADDRESS;

	ipstr = cw_cfg_get(params->cfg,eh->key,NULL);
	if (ipstr==NULL){
		return 0;
	}

	ip.type->from_str(&ip,ipstr);

	int rc = cw_put_local_ip_address(dst,eh->id,
			CAPWAP_ELEM_CAPWAP_LOCAL_IPV4_ADDRESS,
			CAPWAP_ELEM_CAPWAP_LOCAL_IPV6_ADDRESS,
			ip.type->data(&ip),ip.type->len(&ip));

	ip.type->del(&ip);
	return rc;
}
