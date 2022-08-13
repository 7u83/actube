#include "cw.h"
#include "val.h"


static int read_struct(cw_Cfg_t * cfg,const cw_ValStruct_t * stru, const char *pkey, 
	const uint8_t * data, int len)
{
	char key[CW_KTV_MAX_KEY_LEN];
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
					l = len-pos;
				}
			
		}

		l=stru[i].type->read(cfg,key,data+pos,l,stru[i].valguard);

//		result = cw_ktv_add(ktv,key,stru[i].type,stru[i].valguard,data+pos,l);
		
//		stru[i].type->to_str(result,dbstr,100);
//		cw_dbg(DBG_ELEM_DETAIL, "Read (%d): %s: %s",pos,key,dbstr);
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
	NULL

};
