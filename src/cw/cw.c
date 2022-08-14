#include "cw.h"
#include "log.h"
#include "dbg.h"

int cw_in_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	cw_Type_t * type;
	type = (cw_Type_t*)handler->type;
	char mkey[CW_CFG_MAX_KEY_LEN];
	const char *key;



	if (!type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	if (handler->mkkey != NULL){
		handler->mkkey(handler->key,elem_data,elem_len, mkey);
		key = mkey;
	}
	else{
		key = handler->key;
	}


	

/*	cw_dbg(DBG_X, "READ: %s / %s",type->name,key);*/
	type->read(params->cfg, key,elem_data,elem_len,handler->param);
	return CAPWAP_RESULT_SUCCESS;
}



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



int cw_in_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	char key[CW_CFG_MAX_KEY_LEN];
	int radio;
	struct cw_Type *type;
	type = (struct cw_Type*)handler->type;

	if (!type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT;
	}

	
	
	radio=cw_get_byte(elem_data);
	sprintf(key,"radio.%d/%s",radio,handler->key);
	
	/*cw_cfg_set_val(params->cfg, key, 
				handler->type, handler->param, elem_data+1,elem_len-1);*/
	
	type->read(params->cfg, key,elem_data+1,elem_len-1,handler->param);
	return CAPWAP_RESULT_SUCCESS;	
}

int cw_out_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	char key[CW_CFG_MAX_KEY_LEN];
	struct cw_Type * type;

	int len,i,l,start;
	int radios;
	len =0;
	
	radios = cw_cfg_get_byte(params->cfg,"wtp-descriptor/max-radios",0);
	for(i=0;i<radios;i++){
		l=0;

		type = (struct cw_Type*)handler->type;
		start = params->msgset->header_len(handler)+len;

		sprintf(key,"radio.%d/%s",i,handler->key);
		cw_dbg(DBG_X,"KEY: %s",key);

		l += cw_put_byte(dst+start+l,i);
		l += type->write(params->cfg_list, key,dst+start+l,handler->param);
	
		l = params->msgset->write_header(handler,dst+len,l);
		len+=l;

	}
	return len;
}

