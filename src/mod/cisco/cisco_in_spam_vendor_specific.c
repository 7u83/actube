
#include "cw/lw.h"
#include "cw/dbg.h"
#include "cw/proto.h"

int cisco_in_spam_vendor_specific(struct cw_ElemHandler *eh, 
			struct cw_ElemHandlerParams *params, 
			uint8_t * data, int len)
{
	struct cw_ElemHandler *vhandler;
	uint32_t vendor_id, elem_id;
	
	vendor_id = cw_get_dword(data);
	elem_id = cw_get_word(data + 4);

	vhandler = cw_msgset_get_elemhandler(params->conn->msgset,CW_PROTO_LWAPP,vendor_id,elem_id);
	
	
	if (!vhandler) {
                cw_dbg(DBG_WARN,
                       "Can't handle Vendor Specific LWAPP Payload %s/%d, in msg %d (%s) in %s state.",
                       cw_strvendor(vendor_id), elem_id, params->msgdata->type,
                       params->msgdata->name, cw_strstate(params->conn->capwap_state));
                return 0;

	}

	return cw_process_element(params,CW_PROTO_LWAPP,vendor_id,elem_id,data+6,len-6);
}
