
#include "cw.h"
#include "dbg.h"

int cw_out_idx_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	char key[CW_KTV_MAX_KEY_LEN];
	struct cw_KTV * elem, search;
	
	int len,i,l;
	int radios;
	int idx, sr;
	int pos;
	uint8_t * mdst;
	
	len =0;

	i=0;

	pos = 0;
	mdst = dst;
	
	do {
		
		
		sprintf(key,handler->key,i);
		search.key=key;
		/*elem = mavl_get(params->conn->local_cfg, &search);*/
		elem = mavl_get_first(params->conn->local_cfg,&search);
		if(elem != NULL){
			printf("Elem key: %s",elem->key);
		}
		sr = sscanf(elem->key,handler->key,&idx);
		
		if (sr!=1)
			break;
		if (idx<=i)
			break;

		sprintf(key,handler->key,idx);

		printf("Here we are '%s'! --> %d\n",key,idx);


		pos = params->conn->header_len(handler);
		
		pos += cw_put_byte(mdst+pos,idx);
		pos += cw_ktv_write_struct(params->conn->local_cfg,handler->type,key,mdst+pos);
		
		mdst += params->conn->write_header(handler,mdst,pos);
		
	
		


		

		i=idx+1;
		
		
		
		
		
		
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

