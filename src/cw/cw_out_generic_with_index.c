
#include "cw.h"

int cw_out_generic_with_index(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)

{
	char key[CW_KTV_MAX_KEY_LEN];
	int idx;
	cw_KTV_t * result, search;
	int len,start;
	uint8_t * ob;


	idx = 0;
	ob = dst;

	do {
		sprintf(key,"%s.%d",eh->key,idx);
		search.key=key;
		result = mavl_get_first(params->conn->local_cfg,&search);
		if (result==NULL)
			break;
		if (strncmp(result->key,key,strlen(key))!=0)
			break;
	
		start = params->conn->header_len(eh);
		len = cw_put_byte(ob+start,idx);
		
		len += result->type->put(result,ob+start+len);

		ob += params->conn->write_header(eh,ob,len);
		
		idx++;
		
	}while(1);

	return ob-dst;
}

