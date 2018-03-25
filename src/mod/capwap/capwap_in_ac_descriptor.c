
#include "cw/cw.h"

#include "cw/dbg.h"

#include "cw/vendors.h"

int capwap_in_ac_descriptor(struct cw_ElemHandler *eh, 
			struct cw_ElemHandlerParams *params, 
			uint8_t * data, int len)
{
	static struct cw_DescriptorSubelemDef allowed[] = {
		{0,CAPWAP_SUBELEM_AC_HARDWARE_VERSION, "hardware", 1024,1},
		{0,CAPWAP_SUBELEM_AC_SOFTWARE_VERSION, "software", 1024,1},
		{0,0, NULL,0, 0}
	};


	return cw_read_ac_descriptor(params->conn->remote_cfg,eh,params,data,len,allowed);

}
