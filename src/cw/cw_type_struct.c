#include "cw.h"
#include "val.h"
#include "dbg.h"
#include "log.h"


static int read_struct(cw_Cfg_t * cfg,const cw_ValStruct_t * stru, const char *pkey, 
	const uint8_t * data, int len)
{
	char key[CW_CFG_MAX_KEY_LEN];
	int pos, i,l;
	
	
	pos=0; i=0;
	while (stru[i].type != NULL){
		if(stru[i].position!=-1)
			pos=stru[i].position;
		
		if (stru[i].key!=NULL)
			sprintf(key,"%s/%s",pkey,stru[i].key);
		else	
			sprintf(key,"%s",pkey);
		
		switch (stru[i].len){
			case CW_STRUCT_LEN_BYTE:
				/* read len from next byte */
				l = cw_get_byte(data+pos);
				pos ++;
				break;
			case CW_STRUCT_LEN_WORD:
				/* read len from next word */
				l = cw_get_word(data+pos);
				pos ++;
				break;
			case -1:
				l = len-pos;
				break;
			default:
				l = stru[i].len;
				if (pos+l > len){
					l = pos<len ? len-pos : 0 ;
				}
			
		}
		l=stru[i].type->read(cfg,key,data+pos,l,stru[i].valguard);

//		printf("READ STRUCT (%d): %s: %s\n",pos,key,dbstr);
		
		if (stru[i].len==-1){
			///l = result->type->len(result);
		}
		else {
			l = stru[i].len;
		}
	
		if(stru[i].position == -1)
			pos+=l;

		i++;
	}

	return pos;
}


static int bread(cw_Cfg_t *cfg, const char * key, const uint8_t *src, int len, const void *param)
{
	cw_ValStruct_t * stru = (cw_ValStruct_t *) param;

	read_struct(cfg,stru,key,src,len);
	return 1;
}


static int write_struct(cw_Cfg_t ** cfgs,  const cw_ValStruct_t * stru, const char *pkey, 
	uint8_t * dst)
{
	char key[CW_CFG_MAX_KEY_LEN];
	int pos, i;
	const char * result;
	int wrlen;
	int rc;

	cw_Val_t val;
	memset(&val,0,sizeof(cw_Val_t));
	
	pos=0; i=0;
	for(i=0; stru[i].type != NULL;i++){
	
		if (stru[i].position!=-1){
			pos=stru[i].position;
		}
		if (stru[i].len!=-1)
			memset(dst+pos,0,stru[i].len);
		
		
		if (stru[i].key!=NULL)
			sprintf(key,"%s/%s",pkey,stru[i].key);
		else	
			sprintf(key,"%s",pkey);

	//	result = cw_cfg_get_l(cfgs,key,NULL);
	
		rc = cw_cfg_base_exists_l(cfgs,key);
//		cw_dbg(DBG_X,"Base? :'%s', %d\n",key,rc);
		if(result) {
//			char s[2048];
//			result->type->to_str(result,s,2048);
//			printf("Content: '%s'\n",s);
		}	

		
		if (!rc){
			int l;
			cw_log(LOG_ERR,"Can't put %s, no value found, filling with zeros.",key);
			l = stru[i].len;
			if (l==-1)
				l = 0;

			memset(dst+pos,0,l);
			wrlen=l;
		}
		else{
			struct cw_Type * type;
			type = (struct cw_Type *)stru[i].type;
			wrlen = type->write(cfgs,key,dst+pos,stru[i].valguard);


/*			result->valguard=stru[i].valguard;
			if (cw_ktv_cast(result,stru[i].type)==NULL){
				cw_log(LOG_ERR,"Can't cast key '%s' from %s to %s",key,result->type->name,stru[i].type->name);
			}
		
			result->type->put(result,dst+pos);*/
		}
		if (stru[i].len!=-1)
			pos+=stru[i].len;
		else	
			pos+=wrlen; //result->type->len(result);

	}

	return pos;
}




static 	int bwrite(cw_Cfg_t **cfgs, const char *key, uint8_t *dst, const void * param)
{
	return write_struct(cfgs,param,key,dst);

	stop();

/*
	int start;
	int len;
	cw_Val_t search;
	const char *result;

	if (!handler->type){
		cw_log(LOG_ERR,"Can't handle element: %s, no type defined",handler->name);
		return 0;
	}
	
	search.key = (char*)handler->key;
	result = mavl_get_first(params->cfg,&search);
	if (result == NULL ){
		if (params->elemdata->mand)
			cw_log(LOG_ERR,"Can't put mandatory message element %s, no data available",handler->name);
		return 0;
	}
	
	if (strncmp(result->key,handler->key, strlen(handler->key))!=0){
		if (params->elemdata->mand)
			cw_log(LOG_ERR,"Can't put mandatory message element %s, no data available",handler->name);
		return 0;
	}

	start = params->msgset->header_len(handler);

	len = cw_ktv_write_struct(params->cfg,
		params->cfg,
		handler->type,handler->key,dst+start);
	
	return params->msgset->write_header(handler,dst,len);
*/
	
}




const struct cw_Type cw_type_struct = {
	"Struct",		/* name */
	NULL,			/* del */
	NULL,			/* put */
	NULL,			/* get */
	NULL,			/* to_str */
	NULL,			/* from_str */ 
	NULL,			/* len */
	NULL,			/* data */
	NULL,			/* get_type_name */
	NULL,
	bread,
	bwrite

};
