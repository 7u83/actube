#include "cw/lw.h"
#include "cw/cw.h"

#include "capwap_cisco.h"
#include "mod_cisco.h"



int cisco_out_lw_path_mtu(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	char key[CW_KTV_MAX_KEY_LEN];
	int len,max;
		
	sprintf(key,"%s/%s",eh->key,"len");
	len = cw_ktv_get_word(params->conn->local_cfg,key,0);
	if (len == 0)
		return 0;
	sprintf(key,"%s/%s",eh->key,"max");
	max = cw_ktv_get_word(params->conn->local_cfg,key,0);
	
	lw_set_word(dst+16,max);
	lw_set_word(dst+16+2,len);
	memset(dst+16+4,0,len-4);
	
	/* put the lwap elem header */
	lw_set_dword(dst + 10, eh->vendor);
	lw_set_word(dst + 14, eh->id);

	return len + 6 + cw_put_elem_vendor_hdr(dst, eh->vendor, CISCO_ELEM_SPAM_VENDOR_SPECIFIC, len+6);
}
