
#include <stdlib.h>
#include <string.h>


#include "wtpinfo.h"

#include "capwap.h"

#include "cw_util.h"
#include "cw_log.h"



static void wtpinfo_readsubelems_wtp_board_data(struct wtpinfo * wtpinfo,uint8_t * msgelem,int len)
{
	int i=0;
	uint32_t val;
	do {
		val = ntohl(*((uint32_t*)(msgelem+i)));
		int subtype= (val>>16)&0xffff;
		int sublen = val&0xffff;
		i+=4;
		if (sublen+i>len){
			cw_dbg(DBG_ELEM,"WTP Board data sub-element too long, type=%d,len=%d",subtype,sublen);
			return;
		}
		
		cw_dbg(DBG_ELEM,"Reading WTP board data sub-element, type=%d, len=%d",subtype,sublen);

		switch(subtype){
			case CWBOARDDATA_MODELNO:
				bstr_replace(&wtpinfo->model_no,bstr_create(msgelem+i,sublen));
				break;
			case CWBOARDDATA_SERIALNO:
				bstr_replace(&wtpinfo->serial_no,bstr_create(msgelem+i,sublen));
				break;
			case CWBOARDDATA_MACADDRESS:
				wtpinfo->macaddress=realloc(wtpinfo->macaddress,sublen);
				memcpy(wtpinfo->macaddress,msgelem+i,sublen);
				wtpinfo->macaddress_len=sublen;
				break;
			case CWBOARDDATA_BOARDID:
				bstr_replace(&wtpinfo->board_id,bstr_create(msgelem+i,sublen));
				break;
			case CWBOARDDATA_REVISION:
				bstr_replace(&wtpinfo->board_revision,bstr_create(msgelem+i,sublen));
			default:
				break;
		}
		i+=sublen;

	}while(i<len);
}



int wtpinfo_readelem_wtp_board_data(struct wtpinfo *wtpinfo, int type, uint8_t *msgelem, int len)
{

	if (type!=CW_ELEM_WTP_BOARD_DATA)
		return 0;
	if (len<4){
		cw_dbg(DBG_ELEM,"Discarding WTP_BOARD_DATA msgelem, wrong size, type=%d, len=%d\n",type,len);
		return 1;
	}

	wtpinfo->vendor_id = ntohl(*((uint32_t*)msgelem));
	wtpinfo_readsubelems_wtp_board_data(wtpinfo,msgelem+4,len-4);
	return 1;
}


