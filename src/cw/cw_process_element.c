

#include "cw.h"
#include "dbg.h"

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

	handler = cw_msgset_get_elemhandler(params->conn->msgset,0, 0, elem_id);
	if (!handler) {
		cw_dbg(DBG_ELEM_ERR, "Unrecognized message element: %d, ignoring", 
			elem_id);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	elem_data_search.id=elem_id;
	elem_data_search.proto=0;
	elem_data_search.vendor=0;
		
	elem_data = mavl_find_ptr(params->msgdata->elements_tree,&elem_data_search);

	if (!elem_data){
		cw_dbg(DBG_ELEM_ERR, "Element %d - %s, not allowed here", 
			elem_id, handler->name);
		return 0;
	}


	cw_dbg_elem(DBG_ELEM, params->conn, params->msgdata->type, handler, 
				data,len);

	return handler->get(handler, params, data, len);
}
