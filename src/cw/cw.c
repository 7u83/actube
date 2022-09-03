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

//	cw_dbg(DBG_X,"HK: %s",handler->key,);

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
//	cw_dbg(DBG_X, "READ: %s / %s",type->name,key);
	type->read(params->cfg, key,elem_data,elem_len,handler->param);
	return CAPWAP_RESULT_SUCCESS;
}



int cw_out_generic0(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst,const char *key)
{
	int start, len, l;
	
//	cw_dbg(DBG_X,"cw_out_generic (%s)%s",((struct cw_Type*)handler->type)->name,handler->key);

//	cw_dbg(DBG_X,"Generic out!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
//	cw_cfg_dump(params->cfg);
//	cw_dbg(DBG_X,"Generic out!!!!!!!!!!!!!!!!!!!!!!!!!!!! ENDDUMP");
//
//
	if (!params->elemdata->mand){
		if (!cw_cfg_base_exists(params->cfg_list[0],key)){
			cw_dbg(DBG_MSG_COMPOSE,"    Add Elem: %d %d %d %s %s - (bskip)", 
				params->elemdata->proto, 
				params->elemdata->vendor, 
				params->elemdata->id, 
				handler->name, key);
			return 0;
		}
	}	


	start = params->msgset->header_len(handler);
	len = ((const cw_Type_t*)(handler->type))->
		write(params->cfg_list,key,dst+start,handler->param);
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
			       , key
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


int cw_out_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	return cw_out_generic0(handler,params,dst,handler->key);
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

	for (i=0; (i=cw_cfg_get_first_index_l(params->cfg_list,"radio",i))!=-1; i++){
		sprintf(key,"radio.%d/%s",i,handler->key);
		if (!params->elemdata->mand){
			if (!cw_cfg_base_exists(params->cfg_list[0],key)){
				cw_dbg(DBG_MSG_COMPOSE,"    Add Elem: %d %d %d %s %s - (skip)", 
						params->elemdata->proto, 
						params->elemdata->vendor, 
						params->elemdata->id, 
						handler->name, key);
				continue;
			}
		}

		type = (struct cw_Type*)handler->type;
		start = params->msgset->header_len(handler)+len;

		l = type->write(params->cfg_list, key,dst+start+1,handler->param);
		if (l==-1)
			continue;

		l += cw_put_byte(dst+start,i);

		l = params->msgset->write_header(handler,dst+len,l);
		len+=l;
		

	}

	return len;



	
	radios = cw_cfg_get_byte_l(params->cfg_list,"capwap/wtp-descriptor/max-radios",0);
	for(i=0;i<radios;i++){

		type = (struct cw_Type*)handler->type;
		start = params->msgset->header_len(handler)+len;

		sprintf(key,"radio.%d/%s",i,handler->key);
//printf("RADIO KEY: %s\n",key);
		
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



/*
int cw_out_traverse0(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst, int i, const char *current, const char * next, 
			int * stack)
{
	char *sl;
	int l;
	char key[CW_CFG_MAX_KEY_LEN];
	int len;
	len = 0;

printf("Next: %s\n", next);

	sl = strchr(next,'/');
	if (sl==NULL){
		cw_Val_t * result;
		sprintf(key,"%s/%s",current,next);
		result = cw_ktv_base_exists(params->cfg,key);
		if (result != NULL){
			int offset;
			int i,l;
			offset = params->msgset->header_len(handler);
			printf("Yea! We can do it: %s\n",result->key);
			for (i=0;i<stack[0];i++){
				printf("I=%i\n",stack[i+1]);
			}
			l= cw_ktv_write_struct(params->cfg,params->cfg, 
				handler->type,key,dst+offset);
			
			printf("Write struct len %i\n",l);
			
			l=params->msgset->write_header(handler,dst,l);
			printf("header wr len %d\n",l);
			if (handler->patch){
				handler->patch(dst+offset,stack);
			}
			
			return l;
		}
		
		return 0;
	}
	
	strcpy(key,current);

printf("current is %s\n", current);	

	if (key[0!=0])
		strcat(key,"/");
	l = sl - next;
	strncat(key,next,l);
	
	
	printf("Here we are %s\n",key);
	cw_dbg_ktv_dump(params->cfg,DBG_INFO,"start"," ", "end" );
	i=-1;
	while(1){
		char basekey[CW_CFG_MAX_KEY_LEN];
		cw_Val_t * result;
		
		i = cw_ktv_idx_get_next(params->cfg,key,i+1);
		
		if (i==-1)
			break;
		sprintf(basekey,"%s.%d",key,i);
		printf("Our basekey is %s\n",basekey);
		result = cw_ktv_base_exists(params->cfg,basekey);
		if (result == NULL){
			continue;
		}

		stack[0]++;
		stack[stack[0]]=i;
		len += cw_out_traverse0(handler,params,dst+len,-1,basekey,next+l+1, stack);
		printf("Len is now %d\n", len);
	}


	return len;
	
	return 0;
}
*/
int cw_out_traverse(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)

{
/*	char current[CW_CFG_MAX_KEY_LEN];
	int stack[10];
	stack[0]=0;
	current[0]=0;
//	return cw_out_traverse0(handler,params,dst,-1,current,handler->key, stack);*/
	return 0;
}



int walk0(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
		, uint8_t * dst, const char *current, const char * next)
{
        char key[CW_CFG_MAX_KEY_LEN];
        const char *sl;
	int rc;
        int len=0;

        /* Is this the last key ? */
        sl = strchr(next,'/');
        if (sl){
                char basekey[CW_CFG_MAX_KEY_LEN+13];
                int i,l;
                strcpy(key,current);
                l = sl - next;
                strncat(key,next,l);

                for (i=0; (i=cw_cfg_get_first_index_l(params->cfg_list,key,i))!=-1; i++){
                        sprintf(basekey,"%s.%d%c",key,i, *(sl+1) ?'/':'\0');
                        rc = walk0(handler,params,dst,basekey,next+l+1);
			if (rc>0)
				len+=rc;
                }
                return len;

        }

        printf("Final %s [%s]\n",current,next);
	return cw_out_generic0(handler,params,dst,current);

        return 0;

}

int cw_out_generic_walk(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
        char current[CW_CFG_MAX_KEY_LEN];
//        int stack[10];
 //       stack[0]=0;
        current[0]=0;

	return walk0(handler,params,dst,current,handler->key);
}

