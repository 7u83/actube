#include <stdlib.h>

#include "bstr.h"

uint8_t * bstr_replace( uint8_t ** str, uint8_t * data, int len)
{
	if (str)
		free(str);
	
	*str = bstr_create(data,len);	
	return *str;
}
