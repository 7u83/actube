
#include "cw.h"
#include "dbg.h"

int cw_ktv_idx_get_next(mavl_t ktv, const char *key, int n);

int cw_out_radio_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)
{
	cw_dbg(DBG_X,"KEY: %s",handler->key);
	stop();

	int i,l, offset;

	uint8_t * cdst;
	
	cdst = dst;
	

	offset = params->msgset->header_len(handler);

	i=-1;
	while(1){
		char basekey[CW_CFG_MAX_KEY_LEN];
		cw_Val_t * result;
		
		i = cw_ktv_idx_get_next(params->cfg,"radio",i+1);
		if (i==-1)
			break;
		sprintf(basekey,"radio.%d/%s",i,handler->key);
		
		result = cw_ktv_base_exists(params->cfg,basekey);
		if (result == NULL){
			continue;
		}
			
		
		l=0;
		l+=cw_put_byte(cdst+offset+l,i);
		l+= cw_ktv_write_struct(params->cfg,NULL, handler->type,basekey,cdst+offset+l);
		

		cdst+=params->msgset->write_header(handler,cdst,l);
	}
	return cdst-dst;
}


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
}

int cw_out_traverse(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst)

{

	char current[CW_CFG_MAX_KEY_LEN];
	int stack[10];
	stack[0]=0;

	current[0]=0;


	
	return cw_out_traverse0(handler,params,dst,-1,current,handler->key, stack);
}

