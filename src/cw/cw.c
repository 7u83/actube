#include "cw.h"
#include "log.h"
#include "dbg.h"
#include "keys.h"

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
	
//	cw_dbg(DBG_X,"cw_out_generic (%s)%s",((struct cw_Type*)handler->type)->name,handler->key);

//	cw_dbg(DBG_X,"Generic out!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//	cw_cfg_dump(params->cfg);
//	cw_dbg(DBG_X,"Generic out!!!!!!!!!!!!!!!!!!!!!!!!!!!! ENDDUMP");

	start = params->msgset->header_len(handler);
	len = ((const cw_Type_t*)(handler->type))->
		write(params->cfg_list,handler->key,dst+start,handler->param);
//	cw_dbg(DBG_X, "Type result is %d",len);

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
	
	radios = cw_cfg_get_byte_l(params->cfg_list,"wtp-descriptor/max-radios",0);
	for(i=0;i<radios;i++){

		type = (struct cw_Type*)handler->type;
		start = params->msgset->header_len(handler)+len;

		sprintf(key,"radio.%d/%s",i,handler->key);
//		cw_dbg(DBG_X,"KEY: %s",key);

		l = type->write(params->cfg_list, key,dst+start+1,handler->param);
		if (l==-1)
			continue;

		l += cw_put_byte(dst+start,i);

		l = params->msgset->write_header(handler,dst+len,l);
		len+=l;

	}
	return len;
}


int cw_write_header(struct cw_ElemHandler * handler, uint8_t * dst, int len)
{
	if (handler->vendor)
		return len + cw_put_elem_vendor_hdr(dst, handler->vendor, handler->id, len);

	return  len + cw_put_elem_hdr(dst, handler->id, len);
}



int cw_header_len(struct cw_ElemHandler * handler)
{
	return handler->vendor ? 10 : 4;
}



/**
 * Put the "status part" of an an AC Descriptor to memory
 * @param cfg_list Cfg list to read status from
 * @param dst Where to put the status to
 * @param parent_key prefix to each key
 */
int 
cw_put_ac_status(uint8_t *dst, cw_Cfg_t ** cfg_list, const char * parent_key){

	uint8_t *d = dst;
	
	char key[CW_CFG_MAX_KEY_LEN];
	
	/* put statiosn */
	sprintf(key,"%s/%s",parent_key,"stations");
	d += cw_put_word(d,cw_cfg_get_word_l(cfg_list,key,0));

	/* put station limit */
	sprintf(key,"%s/%s",parent_key,"station-limit");
	d += cw_put_word(d,cw_cfg_get_word_l(cfg_list,key,0));

	/* Put number of active WTPs */
	sprintf(key,"%s/%s",parent_key,"active-wtps");
	d += cw_put_word(d,cw_cfg_get_word_l(cfg_list,key,0));

	/* Put max WTPs */
	sprintf(key,"%s/%s",parent_key,"max-wtps");
	d += cw_put_word(d,cw_cfg_get_word_l(cfg_list,key,0));

	/* Put security flags */	
	sprintf(key,"%s/%s",parent_key,"security");
	d += cw_put_byte(d,cw_cfg_get_byte_l(cfg_list,key,0));

	/* Put rmac-filed */
	sprintf(key,"%s/%s",parent_key,"rmac-field");
	d += cw_put_byte(d,cw_cfg_get_byte_l(cfg_list,key,0));
	
	/* reserved field, must be zero - RFC5415 */
	d += cw_put_byte(d,0); 

	sprintf(key,"%s/%s",parent_key,"dtls-policy");
	d += cw_put_byte(d,cw_cfg_get_byte_l(cfg_list,key,0));

	return d - dst;
}

/** 
 * Put a descripter sub element like harware vendor/version etc.
 * Used when putting AC Descriptors or WTP Descriptors
 * @param dst Where to write to
 * @param cfg_list list of cfgs
 * @subelem_id Id of subelement
 * @parent_key parent key
 */
int 
cw_put_descriptor_subelem (uint8_t *dst, cw_Cfg_t ** cfg_list,
                                 int subelem_id, const char * parent_key )
{
	char key[256];
	uint32_t vendor;
	//bstr16_t version;
	const char *vendor_s;

	uint8_t *d;


	/*        d += cw_put_dword(d, bstrv_get_vendor_id(v));
	d += cw_put_dword(d, (subelem_id << 16) | bstrv_len(v));
	d += cw_put_data(d, bstrv_data(v), bstrv_len(v));
	*/
	sprintf (key, "%s/%s", parent_key, CW_SKEY_VENDOR);
	vendor_s = cw_cfg_get_l (cfg_list, key, NULL);
	
	if (vendor_s == NULL) {
		cw_log (LOG_ERR, "Can't put subelem %s, no value of type Dword found.", key);
		return 0;
	}

	vendor = atoi(vendor_s);	



	sprintf (key, "%s/%s", parent_key, CW_SKEY_VERSION);
	cw_Val_t * val = cw_cfg_get_val_l(cfg_list, key, CW_TYPE_BSTR16);

	//version = cw_cfg_get_bstr16 (cfg, key, NULL);


	if (val == NULL) {
		cw_log (LOG_ERR, "Can't put subelem %s, no value of type Bstr16 found.", key);
		return 0;
	}
	
	d = dst;
	
	/* put vendor */
	d += cw_put_dword(d, vendor); //->type->put (vendor, d);
	
	/* put version */
	
	d += cw_put_dword (d, (subelem_id << 16) | val->type->len(val));
//	d += cw_put_bstr16(d, version);
	d += val->type->put(val,d);

	cw_val_destroy(val);

//	free(version);
	
	return d-dst;
}
