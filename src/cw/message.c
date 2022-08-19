#include "cw.h"
#include "log.h"
#include "dbg.h"




int cw_encode_elements(struct cw_ElemHandlerParams *params, mlist_t elements_list, uint8_t * dst)
{
	int len;
	struct mlistelem * elem;

	len =0;
	mlist_foreach(elem,elements_list){
		int l;
		struct cw_ElemData * data;
		struct cw_ElemHandler * handler;
		
		data =  mlistelem_dataptr(elem);
		handler = cw_msgset_get_elemhandler(params->msgset,data->proto,data->vendor,data->id);
		params->elemdata = data;

		if (handler==NULL){
			cw_dbg(DBG_MSG_ASSEMBLY,"    Add Elem: %d %d %d %s", data->proto, data->vendor, data->id, handler->name);
			cw_log(LOG_ERR,"Can't put message element %d %d %d, no handler defined.",
					data->proto,data->vendor,data->id);
			continue;
		}

		if (handler->put == NULL){
			if (data->mand){
				cw_dbg(DBG_MSG_ASSEMBLY,"    Add Elem: %d %d %d %s", data->proto, data->vendor, data->id, handler->name);
				cw_log(LOG_ERR,"Error: Can't add mandatory message element %d - %s, no put method defined",
					handler->id, handler->name);
				
			}
			continue;
		}

		if (!data->mand){
			if (!cw_cfg_base_exists(params->cfg_list[0],handler->key)){
				cw_dbg(DBG_MSG_ASSEMBLY,"    Add Elem: %d %d %d %s - (skip)", 
						data->proto, data->vendor, data->id, handler->name);

				continue;
			}
		}


		l = handler->put(handler,params,dst+len);
		cw_dbg(DBG_MSG_ASSEMBLY,"    Add Elem: %d %d %d %s - (%d bytes)", 
				data->proto, data->vendor, data->id, handler->name,l);

		len += l;
	}

	return len;
}

/**
 * Put a message to a buffer
 * This functions assumes, that a message header is
 * alread initialized in buffer 
 * Message alements are taken fom actiondef in #conn->action
 */
int cw_compose_message(struct cw_Conn *conn, uint8_t * rawout)
{
	char details[1024];
	uint8_t *msgptr,*dst;
	int type;
	struct cw_MsgData * msg;
	int len;
	struct cw_ElemHandlerParams params;

	/* rawout is already initialized, so we can get 
	 * msg type from buffer */
	msgptr = rawout + cw_get_hdr_msg_offset(rawout);
	type = cw_get_msg_type(msgptr);

	/* look for message data */
	msg = cw_msgset_get_msgdata(conn->msgset,type);
	if (msg == NULL){
		cw_log(LOG_ERR,"Error: Can't create message of type %d (%s) - no definition found.",
			type, cw_strmsg(type));
		return CAPWAP_RESULT_MSG_UNRECOGNIZED;
	}

	if (msg->preprocess){
		msg->preprocess(conn);
	}

	cw_dbg(DBG_MSG_ASSEMBLY,"*** Assembling message of type %d (%s) ***", 
			msg->type, msg->name);
	
	dst = msgptr+8;
	len =0;
//cw_dbg(DBG_X,"setting with update CFG");
	params.conn=conn;
	params.cfg_list[0]=conn->update_cfg;
	params.cfg_list[1]=conn->local_cfg;
	params.cfg_list[2]=conn->global_cfg;
	params.cfg_list[3]=NULL;
	params.msgset=conn->msgset;
	params.msgdata=msg;
	params.debug_details=details;
	*details=0;
	len = cw_encode_elements(&params,msg->elements_list,dst);

/*	mlist_foreach(elem,msg->elements_list){
		struct cw_ElemData * data;
		struct cw_ElemHandler * handler;
		
		data =  mlistelem_dataptr(elem);
		handler = cw_msgset_get_elemhandler(conn->msgset,data->proto,data->vendor,data->id);
		cw_dbg(DBG_MSG_ASSEMBLY,"    Add Elem: %d %d %d %s", data->proto, data->vendor, data->id, handler->name);
		if (handler==NULL){
			cw_log(LOG_ERR,"Can't put message element %d %d %d, no handler defined.",
					data->proto,data->vendor,data->id);
			continue;
		}


		if (handler->put == NULL){
			if (data->mand){
				cw_log(LOG_ERR,"Error: Can't add mandatory message element %d - %s, no put method defined",
					handler->id, handler->name);
				
			}
			continue;
		}
*/
/*	
		if (!data->mand){
			if (!cw_cfg_base_exists(params.cfg,handler->key)){
				cw_dbg(DBG_X,"nothing todo");
				continue;
			}
		
		}
		l = handler->put(handler,&params,dst+len);
*/

	/*	if(l>0)
			cw_dbg_elem(DBG_ELEM_OUT,conn,type,handler,dst+len,l);
	*	if (strlen(details)){
			cw_dbg(DBG_ELEM_DETAIL,"  %s",params.debug_details);
		}
	*/	//len += l;
//	}

	cw_set_msg_elems_len(msgptr, len);
	cw_dbg(DBG_MSG_ASSEMBLY,"*** Done assenmbling message of type %d (%s) ***", 
			msg->type, msg->name);
	if (type & 1) {
		/* It's a request, so we have to set seqnum */
		int s = conn_get_next_seqnum(conn);
		cw_set_msg_seqnum(msgptr,s);
	}


	{
	printf ("----------------------------------- redecode -----------------------------\n");
	uint8_t *elems_ptr;

	int offset = cw_get_hdr_msg_offset(rawout);

	uint8_t *msg_ptr = rawout + offset;
	int elems_len = cw_get_msg_elems_len(msg_ptr);
	elems_ptr = cw_get_msg_elems_ptr(msg_ptr);
	cw_Cfg_t * cfg = cw_cfg_create();

	struct cw_ElemHandlerParams params;
	memset(&params,0,sizeof(struct cw_ElemHandlerParams));
	
	params.cfg=cfg;
	params.msgset=conn->msgset;
	params.msgdata=msg;
	params.mand_found = mavl_create_conststr();


	cw_decode_elements( &params, elems_ptr,elems_len);
	cw_cfg_destroy(cfg);
	if (params.mand_found){
		cw_check_missing_mand(msg, params.mand_found,conn->msgset->handlers_by_key);
		mavl_destroy(params.mand_found);
	}

	printf ("----------------------------------- end redecode -----------------------------\n");

	}

	return CAPWAP_RESULT_SUCCESS;

}



int cw_decode_element(struct cw_ElemHandlerParams *params, int proto,
		      int vendor, int elem_id, uint8_t * data, int len)
{

	struct cw_ElemHandler *handler;
	struct cw_ElemData *elem_data, elem_data_search;
	int rc;

	//params->elem = NULL;

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


	if (elem_data->mand && params->mand_found) {
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

	rc = handler->get(handler, params, data, len);

	return rc;
}


int cw_decode_elements(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len)
{
	uint8_t *elem;
//	mavl_t mand_found;
//	mlist_t unrecognized;

	//mand_found = mavl_create_conststr();
	//unrecognized = mlist_create(NULL,NULL,sizeof(uint8_t*));

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
//		params->mand_found=mand_found;
		
		rc = cw_decode_element(params,0,0,elem_id,elem_data,elem_len); 
		

		if (cw_result_is_ok(rc))
			continue;
		
		if (rc == CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT){
			if (params->unrecognized)
				mlist_append(params->unrecognized,&elem);
			continue;
		}
		
		if (rc < 0 ){
			continue;
		}

	}

	return 0;
}
