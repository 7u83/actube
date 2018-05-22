
#include "cw.h"
#include "dbg.h"

int cw_ktv_idx_get_next(mavl_t ktv, const char *key, int n);

int cw_out_radio_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	int i,l, offset;

	uint8_t * cdst;
	
	cdst = dst;
	

	offset = params->conn->header_len(handler);

	i=-1;
	while(1){
		char basekey[CW_KTV_MAX_KEY_LEN];
		cw_KTV_t * result;
		
		i = cw_ktv_idx_get_next(params->conn->local_cfg,"radio",i+1);
		if (i==-1)
			break;
		sprintf(basekey,"radio.%d/%s",i,handler->key);
		
		result = cw_ktv_base_exists(params->conn->local_cfg,basekey);
		if (result == NULL){
			continue;
		}
			
		
		l=0;
		l+=cw_put_byte(cdst+offset+l,i);
		l+= cw_ktv_write_struct(params->conn->local_cfg,NULL, handler->type,basekey,cdst+offset+l);
		

		cdst+=params->conn->write_header(handler,cdst,l);
	}
	return cdst-dst;
}


int cw_out_traverse0(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst, int i, const char *current, const char * next, 
			int * stack)
{
	char *sl;
	int l;
	char key[CW_KTV_MAX_KEY_LEN];
	int len;
	len = 0;
	
	sl = strchr(next,'/');
	if (sl==NULL){
				cw_KTV_t * result;
		sprintf(key,"%s/%s",current,next);
		result = cw_ktv_base_exists(params->conn->local_cfg,key);
		if (result != NULL){
			int offset;
			int i,l;
			offset = params->conn->header_len(handler);
			printf("Yea! We can do it: %s\n",result->key);
			for (i=0;i<stack[0];i++){
				printf("I=%i\n",stack[i+1]);
			}
			l= cw_ktv_write_struct(params->conn->local_cfg,params->conn->default_cfg, 
				handler->type,key,dst+offset);
			
			printf("Write struct len %i\n",l);
			
			l=params->conn->write_header(handler,dst,l);
			printf("header wr len %d\n",l);
			if (handler->patch){
				handler->patch(dst+offset,stack);
			}
			
			return l;
		}
		
		return 0;
	}
	
	strcpy(key,current);
	if (key[0!=0])
		strcat(key,"/");
	l = sl - next;
	strncat(key,next,l);
	
	
	printf("Here we are %s\n",key);
	cw_dbg_ktv_dump(params->conn->local_cfg,DBG_INFO,"start"," ", "end" );
	i=-1;
	while(1){
		char basekey[CW_KTV_MAX_KEY_LEN];
		cw_KTV_t * result;
		
		i = cw_ktv_idx_get_next(params->conn->local_cfg,key,i+1);
		if (i==-1)
			break;
		sprintf(basekey,"%s.%d",key,i);
		printf("Our basekey is %s\n",basekey);
		result = cw_ktv_base_exists(params->conn->local_cfg,basekey);
		if (result == NULL){
			continue;
		}

		stack[0]++;
		stack[stack[0]]=i;
		len += cw_out_traverse0(handler,params,dst+len,-1,basekey,next+l+1, stack);
		printf("Len is now %d\n", len);
	}


	return len;
}

int cw_out_traverse(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)

{

	char current[CW_KTV_MAX_KEY_LEN];
	int stack[10];
	stack[0]=0;

	current[0]=0;
	
	return cw_out_traverse0(handler,params,dst,-1,current,handler->key, stack);
}

