#include <stdint.h>
#include <string.h>
#include <stdlib.h>




uint8_t * bstr_create(uint8_t *data, uint8_t len)
{
	uint8_t * str = malloc(len*sizeof(uint8_t));
	if (!str)	
		return 0;
	*str=len;
	memcpy(str+1,data,len);
	return str;

}
