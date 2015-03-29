
#include "capwap.h"

void cwmsg_addelem_result_code(struct cwmsg *msg,int rc)
{
	uint8_t c[4];
       	*((uint32_t*)c)= htonl(rc);
	cwmsg_addelem(msg,CW_ELEM_RESULT_CODE,c,4);
}

