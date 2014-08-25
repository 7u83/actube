
#include <arpa/inet.h>

#include "capwap.h"
#include "cw_log.h"

#include <stdio.h> //tube



int cw_readelem_vendor_specific_payload(void * data,int msgtype,int elemtype,uint8_t *msgelem, int len)
{
	if (elemtype != CWMSGELEM_VENDOR_SPECIFIC_PAYLOAD)
		return 0;

	if (len < 6){
		cw_dbg(DBG_ERR,"Vendor specific payload msgelem too short, len < 6.");
		return 1;
	}

//	uint32_t vendor_id = ntohl( *((uint32_t*)msgelem) );


	return 1;	
}

