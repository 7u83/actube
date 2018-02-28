#include "bstr.h"

uint8_t * bstrv_create(uint32_t vendor_id, uint8_t *data, uint8_t len)
{
	uint8_t * str = malloc(bstrv_size(len));
	if (!str)	
		return 0;

	bstrv_set_vendor_id(str,vendor_id);
	bstrv_set_len(str,len);
	memcpy(bstrv_data(str),data,len);
	*(bstrv_data(str)+bstrv_len(str))=0;
	return str;

}


