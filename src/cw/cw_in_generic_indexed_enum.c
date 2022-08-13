

#include "cw.h"
#include "cfg.h"



static const cw_ValEnum_t * get_enum(const cw_ValEnum_t * e, int val){
	int i;
	for (i=0; e[i].type != NULL; i++ ){
		if (e[i].value==val){
			return &(e[i]);
		}
	}
	return NULL;
}

int cw_in_generic_indexed_enum(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len)
{
	int val;
	int l,f;
	const cw_ValEnum_t * e;
	const cw_ValIndexed_t * ie;
	
	char key[CW_CFG_MAX_KEY_LEN];
	struct cw_ElemHandler thandler;
	
	ie = handler->type;
	
	val = cw_get_byte(elem_data+ie->idxpos);
	e = get_enum(ie->type,val);
	
	f=0;
	if (ie->idxpos==0){
		l=1;
		f=1;
	}
	if (ie->idxpos==elem_len-1){
		l=1;
	}
	
	if (e!=NULL){
		sprintf(key,"%s/%s",handler->key,e->name);
	}
	else{
		sprintf(key,"%s/%u",handler->key,val);
	}
	
	memset(&thandler,0,sizeof(thandler));
	
	thandler.key=key;

	thandler.type=e->type;
	thandler.param=e->param;
	return e->fun_in(&thandler,params,elem_data+f,elem_len-l);

}
