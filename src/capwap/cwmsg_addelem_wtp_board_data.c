
#include <stdlib.h>
#include <string.h>

#include "capwap.h"


void cwmsg_addelem_wtp_board_data(struct cwmsg * cwmsg, struct wtpinfo * wtpinfo)
{
	uint8_t msg[1030];
	*((uint32_t*)msg)=htonl(wtpinfo->vendor_id);

	int l;
	int len=4;

	if (wtpinfo->model_no){
		l=strlen((char*)wtpinfo->model_no);
		*((uint32_t*)(msg+len))=htonl(CWBOARDDATA_MODELNO<<16|l);
		memcpy(msg+len+4,wtpinfo->model_no,l);
		len+=l+4;
	}

	if (wtpinfo->serial_no){
		l=strlen((char*)wtpinfo->serial_no);
		*((uint32_t*)(msg+len))=htonl(CWBOARDDATA_SERIALNO<<16|l);
		memcpy(msg+len+4,wtpinfo->serial_no,l);
		len+=l+4;
	}

	if (wtpinfo->macaddress){
		*((uint32_t*)(msg+len))=htonl(CWBOARDDATA_MACADDRESS<<16|wtpinfo->macaddress_len);
		memcpy(msg+len+4,wtpinfo->macaddress,wtpinfo->macaddress_len);
		len+=wtpinfo->macaddress_len+4;
	}

	cwmsg_addelem(cwmsg,CWMSGELEM_WTP_BOARD_DATA,msg,len);
}
