
#include "cw.h"


#include "dbg.h"
#include "log.h"
#include "msgset.h"
#include "ktv.h"


int cw_out_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{

	struct cw_KTV * elem, search;
	int start, len, l;

	/* Get the element */
/*	search.key=(char*)handler->key;
	elem = mavl_get(params->conn->local_cfg, &search);
*/	
	elem = cw_ktv_get(params->conn->local_cfg,handler->key,NULL);
	
/*	if (elem == NULL && params->conn->default_cfg !=NULL)
		elem = mavl_get(params->conn->default_cfg, &search);
*/
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

	/* Size for msg elem header depends on 
	   vendor specific payload */
	/* start = handler->vendor ? 10 : 4; */
	start = params->conn->header_len(handler);
	
	if (cw_ktv_cast(elem,handler->type)==NULL){
		cw_log(LOG_ERR,"Can't put element '%s'- can't cast from %s to %s for key: %s", handler->name, 
			elem->type->name, ((const cw_Type_t*)handler->type)->name, handler->key);
		return 0;
	}
	
	len = ((const cw_Type_t*)(handler->type))->put(elem,dst+start);

/*	((const cw_Type_t*)(handler->type))->to_str(elem,detail,120);
	sprintf(params->debug_details, "  Value = %s", detail);
	params->elem = elem;*/

/*	if (handler->vendor)
		return len + cw_put_elem_vendor_hdr(dst, handler->vendor, handler->id, len);

	l = len + cw_put_elem_hdr(dst, handler->id, len); */
	l = params->conn->write_header(handler,dst,len);
	
	cw_dbg_elem(DBG_ELEM_OUT,params->conn,params->msgdata->type,handler,dst,l);
	return l;
}




