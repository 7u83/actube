#include "cw.h"
#include"dbg.h"
#include "cfg.h"


int cw_out_generic_indexed_enum(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
//	cw_dbg(DBG_X,"Fix: cw_out_generic_indexed_enum %s",handler->key);
//	stop();


	char key[CW_CFG_MAX_KEY_LEN];
	int i;
//	cw_Val_t * result;
	int len,start;
	uint8_t * ob;
	const cw_ValIndexed_t *ie;
	cw_ValEnum_t * e;
	
	ie = handler->type;


	i = 0;
	ob = dst;
	
	e = ie->type;
	
	for(i=0; e[i].name!=NULL; i++) {
		int b;
		sprintf(key,"%s/%s",handler->key,e[i].name);

cw_dbg(DBG_X,"Her is the Key: %s %s\n",key,e[i].name);

		b = cw_cfg_base_exists_l(params->cfg_list,handler->key);
		if (!b){
			//stop();
			continue;
		}
		start = params->msgset->header_len(handler);
		len = 0;
		if (ie->idxpos==0)
			len = 1;
	
		if (e[i].fun_out==NULL){
			stop();
//			result = 			
//			len += result->type->put(result,ob+start+len);
		}
		else{
			len += ((cw_Type_t*)e[i].type)->write(params->cfg_list,key,ob+start+len,e[i].param);
//			len += cw_ktv_write_struct(params->cfg,
//			NULL,e[i].type,key,ob+start+len);
		}
			
		cw_set_byte(ob+start+ie->idxpos,e[i].value);
		if (ie->idxpos==len)
			len++;

		ob += params->msgset->write_header(handler,ob,len);
	//	cw_val_destroy(result);
	}

	return ob-dst;
	
	return 0;

}
