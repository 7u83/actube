
#include "lwapp.h"

int lw_readelem_vendor_specific(void * data,int msgtype,int elemtype,uint8_t *msgelem, int len)
{
	if (elemtype != LW_ELEM_VENDOR_SPECIFIC)
		return 0;

	return 1;
}

