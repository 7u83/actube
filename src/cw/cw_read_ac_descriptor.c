
#include "cw.h"
#include "dbg.h"
#include "val.h"

static struct cw_DescriptorSubelemDef allowed_default[] = {
	{0,CAPWAP_SUBELEM_AC_HARDWARE_VERSION, "hardware", 1024,1},
	{0,CAPWAP_SUBELEM_AC_SOFTWARE_VERSION, "software", 1024,1},
	{0,0, NULL,0, 0}
};

cw_ValStruct_t acstatus [] = {
	/* type	        key	   	len,	pos */ 
	{CW_TYPE_WORD, "stations", 	2,	-1},
	{CW_TYPE_WORD, "station-limit", 2,	-1},
	{CW_TYPE_WORD, "active-wtps",	2,	-1},
	{CW_TYPE_WORD, "max-wtps",	2,	-1},
	{CW_TYPE_BYTE, "security",	1,	-1},
	{CW_TYPE_BYTE, "r-mac-field",	1,	-1},
	{CW_TYPE_BYTE, "reserved1",	1,	-1},
	{CW_TYPE_BYTE, "dtls-policy",	1,	-1},
	{NULL,NULL,0,0}
};




int cw_read_ac_descriptor(mavl_t store,
			   struct cw_ElemHandler *eh,
			   struct cw_ElemHandlerParams * params,
			   uint8_t *data, int len, 
			struct cw_DescriptorSubelemDef *allowed)
{

	CW_TYPE_STRUCT->read(params->cfg,eh->key,data,len,acstatus);
//	cw_ktv_read_struct(params->remote_cfg,acstatus,eh->key,data,len);

	if (!allowed)
		allowed=allowed_default;

	return cw_read_descriptor_subelems(store, eh->key, data + 12, len - 12, allowed);
	
}


