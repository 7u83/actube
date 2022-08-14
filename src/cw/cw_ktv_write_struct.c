#include "val.h"
#include "dbg.h"
#include "log.h"
#include "cfg.h"

int cw_ktv_write_struct(mavl_t ktv, mavl_t def, const cw_ValStruct_t * stru, const char *pkey, 
	uint8_t * dst)
{
	char key[CW_CFG_MAX_KEY_LEN];
	int pos, i;
	cw_Val_t * result;
	
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

printf("Get Key: %s\n",key);
		result = cw_ktv_get(ktv,key,NULL);

		if(result)
		{
			char s[129];
			result->type->to_str(result,s,128);
			printf("Content: '%s'\n",s);
		}	

		if (result == NULL && def != NULL){
			result = cw_ktv_get(def,key,NULL);
		}

		
		if (result == NULL){
			cw_log(LOG_ERR,"Can't put %s, no value found, filling zero.",key);
			memset(dst+pos,0,stru[i].len);
		}
		else{
			result->valguard=stru[i].valguard;
			if (cw_ktv_cast(result,stru[i].type)==NULL){
				cw_log(LOG_ERR,"Can't cast key '%s' from %s to %s",key,result->type->name,stru[i].type->name);
			}
/*			if (strcmp(stru[i].type->name,result->type->name)){
				
				
				
				printf("Type mismatch: %s != %s\n",stru[i].type->name,result->type->name);
				if (stru[i].type->cast != NULL){
					if (!stru[i].type->cast(result)){
						cw_log(LOG_ERR,"Can't cast '%s' from %s to %s",key,result->type->name,stru[i].type->name);
						exit(0);
					}
				}
							

			}
*/		
			result->type->put(result,dst+pos);
		}
		if (stru[i].len!=-1)
			pos+=stru[i].len;
		else	
			pos+=result->type->len(result);

	}

	return pos;
}
