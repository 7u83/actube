#include "bstr.h"

uint8_t * bstr16_create(const uint8_t *data, uint16_t len)
{
	uint8_t * str = malloc(2+len*sizeof(uint8_t));
	if (!str)	
		return 0;
	*((uint16_t*)str)=len;
	memcpy(str+2,data,len);
	return str;
}


