
#include "cw.h"
#include "dbg.h"
#include "log.h"

/** 
 * @file 
 * @brief Implementation of Vendor Specific Payload
 */

/**
 * Default handler for Vendor Specific Payload message elements.
 */
int cw_in_vendor_specific_payload(struct cw_ElemHandler *handler, 
				struct cw_ElemHandlerParams *params,
				  uint8_t * data, int len)
{

	struct cw_ElemHandler *vhandler;
	
	uint32_t vendor_id, elem_id;
	
	vendor_id = cw_get_dword(data);
	elem_id = cw_get_word(data + 4);

	printf("Spec handler looking for %d %d %d\n",0,vendor_id,elem_id);
	vhandler = cw_msgset_get_elemhandler(params->conn->msgset,0,vendor_id,elem_id);
	
	
	if (!vhandler) {
		cw_dbg(DBG_WARN,
		       "Can't handle Vendor Specific Payload %s/%d, in msg %d (%s) in %s state.",
		       cw_strvendor(vendor_id), elem_id, params->msgdata->type,
		       params->msgdata->name, cw_strstate(params->conn->capwap_state));
		return 0;
	}
	
	cw_dbg_elem(DBG_ELEM,params->conn,0,vhandler,data+6,len-6);
	vhandler->get(vhandler, params, data+6, len-6);

	return 1;

}
