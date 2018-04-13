
#include "cw.h"
#include "dbg.h"

int cw_out_idx_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	char key[CW_KTV_MAX_KEY_LEN];
	struct cw_KTV * elem, search;
	
	int len,i,l;
	int radios;
	int fint, sr;
	
	len =0;

	i=0;

	
	do {
		sprintf(key,handler->key,i);
		search.key=key;
		/*elem = mavl_get(params->conn->local_cfg, &search);*/
		elem = mavl_get_first(params->conn->local_cfg,&search);
		if(elem != NULL){
			printf("Elem key: %s",elem->key);
		}
		sr = sscanf(elem->key,handler->key,&fint);
		
		if (sr!=1)
			break;
		if (fint<=i)
			break;

		printf("Here we are '%s'! --> %d\n",key,fint);
		i=fint+1;
		
	}while(1);
	
	exit(0);
	
	radios = cw_ktv_get_byte(params->conn->local_cfg,"wtp-descriptor/max-radios",0);

	for(i=1;i<radios+1;i++){
		l = cw_write_radio_element(handler,params,i,dst+len);
		cw_dbg_elem(DBG_ELEM_OUT,params->conn,params->msgdata->type,handler,dst,l);
		len+=l;
	}
	return len;
}

