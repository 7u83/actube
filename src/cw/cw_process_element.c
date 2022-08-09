

#include "cw.h"
#include "dbg.h"
#include "log.h"

/**
 * @brief Process a CAPWAP message element
 * @param conn
 * @param msgdata
 * @param handler
 * @param elems_ptr
 * @param elems_len
 * @return Result Code as defined in RFC5415 
 */
int cw_process_element(struct cw_ElemHandlerParams *params, int proto, int vendor,int elem_id,
		uint8_t  * data, int len){
	
	struct cw_ElemHandler * handler;
	struct cw_ElemData * elem_data, elem_data_search;
	int rc;
	
//	params->elem=NULL;
	
	/* try to retrieve a handler for this message element */
	handler = cw_msgset_get_elemhandler(params->msgset,proto, vendor, elem_id);
	if (!handler) {
		
		cw_dbg(DBG_ELEM_ERR, "Unrecognized message element: %d, ignoring", 
			elem_id);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	/* check if this message element in the current message allowed */
	elem_data_search.id=elem_id;
	elem_data_search.proto=proto;
	elem_data_search.vendor=vendor;
	elem_data = mavl_get(params->msgdata->elements_tree,&elem_data_search);
	if (!elem_data){
		cw_dbg(DBG_ELEM_ERR, "Element %d - %s, not allowed here", 
			elem_id, handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	/* check the length of the message */
	if (len < handler->min_len) {
		cw_dbg(DBG_ELEM_ERR,
		       "%d (%s) message element too short, len=%d, min len=%d",
		       handler->id, handler->name, len,
		       handler->min_len);
		return -1;
	}


	if (elem_data->mand){
		mavl_add_str(params->mand_found,handler->key);
	}


	if (len > handler->max_len && handler->max_len) {
		cw_dbg(DBG_ELEM_ERR,
		       "%d (%s) message element too big, len=%d, max len=%d", handler->id,
		       handler->name, len, handler->max_len);
		/* TODO XXX truncate the element instead of return */
		return -1;
	}

	/*cw_dbg_elem(DBG_ELEM_IN, params->conn, params->msgdata->type, handler,
				data,len);*/
	cw_dbg_elem(DBG_ELEM_IN, NULL, params->msgdata->type, handler,
				data,len);

	if (handler->get == NULL){
		cw_log(LOG_ERR,"No get method defined for %d %s",handler->id,handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}


	rc = handler->get(handler, params, data, len);


	return rc;
}
