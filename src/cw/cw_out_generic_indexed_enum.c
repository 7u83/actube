#include "cw.h"

int cw_out_generic_indexed_enum(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	char key[CW_KTV_MAX_KEY_LEN];
	int i;
	cw_KTV_t * result;
	int len,start;
	uint8_t * ob;
	const cw_KTVIndexed_t *ie;
	cw_KTVEnum_t * e;
	
	ie = handler->type;


	i = 0;
	ob = dst;
	
	e = ie->type;
	
	for(i=0; e[i].name!=NULL; i++) {
		sprintf(key,"%s/%s",handler->key,e[i].name);

//printf("Her is the Key: %s - %s\n",key, );

		result = cw_ktv_base_exists(params->local_cfg,key);
		if (result==NULL)
			continue;
		start = params->msgset->header_len(handler);
		len = 0;
		if (ie->idxpos==0)
			len = 1;
	
		if (e[i].fun_out==NULL)
			len += result->type->put(result,ob+start+len);
		else
			len += cw_ktv_write_struct(params->local_cfg,
			NULL,e[i].type,key,ob+start+len);
			
		cw_set_byte(ob+start+ie->idxpos,e[i].value);
		if (ie->idxpos==len)
			len++;

		ob += params->msgset->write_header(handler,ob,len);
		
	}

	return ob-dst;

}
