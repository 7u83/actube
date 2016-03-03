#include "capwap.h"
#include "cw_log.h"

int cw_readmsg_configuration_update_request(uint8_t *elems,int elems_len)
{
	uint8_t * elem;

	cw_foreach_elem(elem,elems,elems_len){

		int type = cw_get_elem_type(elem);
		int len = cw_get_elem_len(elem);
		uint8_t *data = cw_get_elem_data(elem);

		cw_dbg_msgelem(CW_MSG_CONFIGURATION_STATUS_REQUEST, type, data, len);


//		cw_readelem_vendor_specific_payload(elem,CW_MSG_CONFIGURATION_STATUS_RESPONSE, type,data,len );

	}


}
