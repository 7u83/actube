#include "cw.h"


static const cw_KTVEnum_t * get_enum(const cw_KTVEnum_t * e, int val){
	int i;
	for (i=0; e[i].type != NULL; i++ ){
		if (e[i].value==val){
			return &(e[i]);
		}
	}
	return NULL;
}


int cw_in_generic_enum(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	int val;
	const cw_KTVEnum_t * e;
	char key[CW_KTV_MAX_KEY_LEN];
	struct cw_ElemHandler thandler;
	
	val = cw_get_byte(elem_data+1);
	e = get_enum(handler->type,val);
	if (e!=NULL){
		sprintf(key,"%s/%s",handler->key,e->name);
	}
	else{
		sprintf(key,"%s/%u",handler->key,val);
	}
	
	thandler.type=e->type;
	thandler.key=key;
	return e->fun(&thandler,params,elem_data,elem_len-1);
	
}