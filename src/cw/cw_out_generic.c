
#include "cw.h"


#include "dbg.h"
#include "log.h"
#include "msgset.h"
#include "ktv.h"


int cw_out_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	char detail[128];
	struct cw_KTV * elem, search;
	int start, len;
	/* Get the element to put */

	search.key=(char*)handler->key;
	elem = mavl_get(params->conn->local_cfg, &search);

	/* Size for msg elem header depends on 
	   vendor specific payload */
	start = handler->vendor ? 10 : 4;

	if (elem == NULL) {
		const char *vendor="";
		if ( handler->vendor ) {
			vendor=cw_strvendor(handler->vendor);
		}
		if ( params->elemdata->mand) {
			cw_log(LOG_ERR,
			       "Can't put mandatory element %s %d-(%s) into %s. No value for '%s' found.",
				vendor, handler->id, handler->name, params->msgdata->name
			       , handler->key
			    );
		}
		else{
/*			cw_dbg(DBG_WARN,"No output for element %s%d -(%s) in %s. Item %s not found.",
				vendor,
			       a->elem_id, cw_strelemp(conn->actions, a->elem_id)
			       , cw_strmsg(a->msg_id),a->item_id);
*/
		}
		return 0;
	} 
	len = handler->type->put(elem,dst+start);
	
	
	handler->type->to_str(elem,detail,120);
	sprintf(params->debug_details, "  Value = %s", detail);
	

	
	params->elem = elem;

	/*(cw_put_mbag_item(dst + start, item);*/

	if (handler->vendor)
		return len + cw_put_elem_vendor_hdr(dst, handler->vendor, handler->id, len);

	return len + cw_put_elem_hdr(dst, handler->id, len);
}




