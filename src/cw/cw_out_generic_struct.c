#include "capwap.h"
#include "msgset.h"
#include "ktv.h"
#include "log.h"
#include "cw.h"

int cw_out_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	int start;
	int len,l;
	
	start = handler->vendor ? 10 : 4;
	
	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}
	
	len = cw_ktv_write_struct(params->conn->local_cfg,handler->type,handler->key,dst+start);
	
	if (handler->vendor)
		return len + cw_put_elem_vendor_hdr(dst, handler->vendor, handler->id, len);

	l = len + cw_put_elem_hdr(dst, handler->id, len);

	return l;
	
	
}
