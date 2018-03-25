
#include "cw/cw.h"

#include "cw/dbg.h"

#include "cw/vendors.h"

int cisco_in_ac_descriptor(struct cw_ElemHandler *eh, 
			struct cw_ElemHandlerParams *params, 
			uint8_t * data, int len)
{
	static struct cw_DescriptorSubelemDef allowed[] = {
		{CW_VENDOR_ID_CISCO,0, "hardware", 1024,1},
		{CW_VENDOR_ID_CISCO,1, "software", 1024,1},
		{0,0, NULL,0, 0}
	};

	return cw_read_ac_descriptor(params->conn->remote_cfg,eh,params,data,len,allowed);


	/*return cw_read_ac_descriptor(conn->config,data,len,allowed);*/

}
