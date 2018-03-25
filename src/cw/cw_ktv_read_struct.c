#include "ktv.h"
#include "dbg.h"

int cw_ktv_read_struct(mavl_t ktv,cw_KTVStruct_t * stru, const char *pkey, 
	uint8_t * data, int len)
{
	char key[CW_KTV_MAX_KEY_LEN];
	int pos, i;
	cw_KTV_t * result;
	
	
	pos=0; i=0;
	while (stru[i].type != NULL){
		char dbstr[100];
		
		sprintf(key,"%s/%s",pkey,stru[i].key);
		result = cw_ktv_add(ktv,key,stru[i].type,data+pos,stru[i].len);
		
		stru[i].type->to_str(result,dbstr,100);
		cw_dbg(DBG_ELEM_DETAIL, "Read (%d): %s: %s",pos,key,dbstr);
	
		if(stru[i].position == -1)
			pos+=stru[i].len;
		else
			pos=stru[i].position;
		i++;
	}

	return pos;
}
