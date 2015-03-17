
#include <arpa/inet.h>

#include "capwap.h"
#include "lwapp.h"

#include "cw_log.h"

#include <stdio.h> 



int cw_readelem_cisco_payload(void *data,int msgtype,int elem_id,uint8_t *msgelem, int len)
{


	switch (msgtype) {
		case CWMSG_CONFIGURATION_STATUS_REQUEST:
		{
			if (lw_readelem_wtp_board_data((struct wtpinfo*)data,elem_id,msgelem,len))
				return 1;

			
			return 0;
		}
		case CWMSG_DISCOVERY_REQUEST:
		{
			struct wtpinfo *  wtpinfo = (struct wtpinfo*)data;
			if (lw_readelem_wtp_name(&wtpinfo->name,elem_id,msgelem,len))
				return 1;

		}


		default:
			return 0;



	}	

	return 0;

}



int cw_readelem_vendor_specific_payload(void * data,int msgtype,int elemtype,uint8_t *msgelem, int len)
{
	if (elemtype != CWMSGELEM_VENDOR_SPECIFIC_PAYLOAD)
		return 0;

	if (len < 6){
		cw_dbg(DBG_ERR,"Vendor specific payload msgelem too short, len < 6.");
		return 1;
	}


	uint32_t vendor_id = ntohl( *((uint32_t*)msgelem) );
	uint16_t elem_id = ntohs( *( (uint16_t*)(msgelem+4) ));
	int elem_len = len - 6;

	switch (vendor_id) {

		case CW_VENDOR_ID_CISCO:
			return cw_readelem_cisco_payload(data,msgtype,elem_id,msgelem+6,elem_len);

	}


	return 1;	
}

