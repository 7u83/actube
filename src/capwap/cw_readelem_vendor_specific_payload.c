/**
 * @file
 * @brief Implements cw_readelem_vendor_specific_payload 
 */


#include <arpa/inet.h>

#include "capwap.h"
#include "lwapp.h"

#include "cw_log.h"

#include <stdio.h> 



int cw_readelem_cisco_payload(void *data,int msgtype,int elem_id,uint8_t *msgelem, int len)
{


	switch (msgtype) {
		case CW_MSG_CONFIGURATION_STATUS_REQUEST:
		{
			if (lw_readelem_wtp_board_data((struct wtpinfo*)data,elem_id,msgelem,len))
				return 1;

			
			return 0;
		}
		case CW_MSG_DISCOVERY_REQUEST:
		{
			struct wtpinfo *  wtpinfo = (struct wtpinfo*)data;
			if (lw_readelem_wtp_name(&wtpinfo->name,elem_id,msgelem,len))
				return 1;

		}

		case CW_MSG_CONFIGURATION_STATUS_RESPONSE:
		{

			struct radioinfo ri;
			//lw_read_elem_wtp_wlan_radio_
			//cw_read


		}




		default:
			return 0;



	}	

	return 0;

}


/**
 * Read a CAPWAP Vendor Specific Payload message element.
 * @param data pointer to data where the results can be stored
 * @param msgtype type of message in which the element was found
 * @param elemtype element type, must be CW_ELEM_VENDOR_PECIFIC_PAYLOAD
 * @param msgelem pointer to message elemenet data
 * @param len length of message element data
 * @return 1 = successful read\n 0 = no vendor specific payload element
 */

int cw_readelem_vendor_specific_payload(void * data,int msgtype,int elemtype,uint8_t *msgelem, int len)
{
	if (elemtype != CW_ELEM_VENDOR_SPECIFIC_PAYLOAD)
		return 0;

	if (len < 6){
		cw_dbg(DBG_ERR,"Vendor specific payload msgelem too short, len < 6.");
		return 1;
	}


	uint32_t vendor_id = cw_get_dword(msgelem); 
	uint16_t elem_id = cw_get_word(msgelem+4); 
	int elem_len = len - 6;

	switch (vendor_id) {

		case CW_VENDOR_ID_CISCO:
			return cw_readelem_cisco_payload(data,msgtype,elem_id,msgelem+6,elem_len);
		default:
			cw_dbg(DBG_ERR,"Can't read vendor specific payload. Vendor ID %d",vendor_id);

	}


	return 1;	
}

