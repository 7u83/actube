
#include "cw.h"


#include "dbg.h"
#include "log.h"
#include "msgset.h"
#include "val.h"


int cw_out_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{

	int start, len, l;

//	cw_dbg(DBG_X,"Generic out!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//	cw_cfg_dump(params->cfg);
//	cw_dbg(DBG_X,"Generic out!!!!!!!!!!!!!!!!!!!!!!!!!!!! ENDDUMP");

	start = params->msgset->header_len(handler);
	len = ((const cw_Type_t*)(handler->type))->
		write(params->cfg_list,handler->key,dst+start,handler->param);


	if (len == -1) {
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

	l = params->msgset->write_header(handler,dst,len);

	return l;
}




