#include "cw.h"
#include "dbg.h"
#include "log.h"
#include "msgset.h"

int cw_decode_element(struct cw_ElemHandlerParams *params, int proto,
		      int vendor, int elem_id, uint8_t * data, int len)
{

	struct cw_ElemHandler *handler;
	struct cw_ElemData *elem_data, elem_data_search;
	int rc;

	params->elem = NULL;

	/* try to retrieve a handler for this message element */
	handler =
	    cw_msgset_get_elemhandler(params->msgset, proto, vendor,
				      elem_id);
	if (!handler) {

		cw_dbg(DBG_ELEM_ERR,
		       "Unrecognized message element: %d, ignoring", elem_id);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	/* check if this message element in the current message allowed */
	elem_data_search.id = elem_id;
	elem_data_search.proto = proto;
	elem_data_search.vendor = vendor;
	elem_data = mavl_get(params->msgdata->elements_tree, &elem_data_search);
	if (!elem_data) {
		cw_dbg(DBG_ELEM_ERR, "Element %d - %s, not allowed here",
		       elem_id, handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	/* check the length of the message */
	if (len < handler->min_len) {
		cw_dbg(DBG_ELEM_ERR,
		       "%d (%s) message element too short, len=%d, min len=%d",
		       handler->id, handler->name, len, handler->min_len);
		return -1;
	}


	if (elem_data->mand) {
		mavl_add_str(params->mand_found, handler->key);
	}


	if (len > handler->max_len && handler->max_len) {
		cw_dbg(DBG_ELEM_ERR,
		       "%d (%s) message element too big, len=%d, max len=%d",
		       handler->id, handler->name, len, handler->max_len);
		/* TODO XXX truncate the element instead of return */
		return -1;
	}

	cw_dbg_elem(DBG_ELEM_IN, NULL, params->msgdata->type, handler,
		    data, len);

	if (handler->get == NULL) {
		cw_log(LOG_ERR, "No get method defined for %d %s", handler->id,
		       handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	printf ("USING HANDLERi TO DECODE: %d %s\n",handler->id,handler->name);
	rc = handler->get(handler, params, data, len);


	return rc;
}


int cw_decode_elements(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len)
{
	uint8_t *elem;
	mavl_t mand_found;
	mlist_t unrecognized;

	mand_found = mavl_create_conststr();
	unrecognized = mlist_create(NULL,NULL,sizeof(uint8_t*));

	cw_foreach_elem(elem, elems_ptr, elems_len) {
		int rc;

		int elem_len, elem_id, max_len;
		uint8_t * elem_data;


		elem_len = cw_get_elem_len(elem);
		elem_data=cw_get_elem_data(elem);
		elem_id = cw_get_elem_id(elem);
		
		max_len=elems_len-(elem_data-elems_ptr);
		if (elem_len > max_len){
			cw_dbg(DBG_RFC,
			"Messag element claims size of %d bytes, but only %d bytes are left in the payload, truncating.",
			elem_len,max_len-4);
		}
	
		

		params->from=NULL; /*from;*/
		params->mand_found=mand_found;
		
		rc = cw_decode_element(params,0,0,elem_id,elem_data,elem_len); 
		

		if (cw_result_is_ok(rc))
			continue;
		
		if (rc == CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT){
			mlist_append(unrecognized,&elem);
			continue;
		}
		
		if (rc < 0 ){
			continue;
		}

	}

}
