#include "ktv.h"
#include "dbg.h"

int cw_ktv_read_struct(mavl_t ktv,const cw_KTVStruct_t * stru, const char *pkey, 
	uint8_t * data, int len)
{
	char key[CW_KTV_MAX_KEY_LEN];
	int pos, i,l;
	cw_KTV_t * result;
	
	if (strcmp(pkey,"radio.0/cisco/wtp-radio-config")==0)
		i = 99;
	
	
	pos=0; i=0;
	while (stru[i].type != NULL){
		char dbstr[100];
		if(stru[i].position!=-1)
			pos=stru[i].position;
		
		if (stru[i].key!=NULL)
			sprintf(key,"%s/%s",pkey,stru[i].key);
		else	
			sprintf(key,"%s",pkey);
		
		switch (stru[i].len){
			case CW_KTVSTRUCT_L8:
				l = cw_get_byte(data+pos);
				pos ++;
				break;
			case CW_KTVSTRUCT_L16:
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

		result = cw_ktv_add(ktv,key,stru[i].type,stru[i].valguard,data+pos,l);
		
		stru[i].type->to_str(result,dbstr,100);
		cw_dbg(DBG_ELEM_DETAIL, "Read (%d): %s: %s",pos,key,dbstr);
		
		if (stru[i].len==-1)
			l = result->type->len(result);
		else 
			l = stru[i].len;
	
		if(stru[i].position == -1)
			pos+=l;

		i++;
	}

	return pos;
}
