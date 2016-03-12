#include "bstr.h"
#include "format.h"


uint8_t * bstr16_create_from_str(const char *s)
{
	int l = strlen(s);
	if (s[0]!='.')
		return bstr16_create((uint8_t*)s,l);

	if (l<=2)
		return bstr16_create((uint8_t*)s,l);

	if (s[1]=='.')
		return bstr16_create((uint8_t*)s+1,l-1);

	if (s[1]!='x')
		return bstr16_create((uint8_t*)s,l);

	/* the string starts with ".x" - read hexbytes */
	l-=2;
	int msize=l/2;	
	if(l&1)
		msize++;
	uint8_t * mem = malloc(2+msize);
	*((uint16_t*)mem)=msize;
	format_scan_hex_bytes(mem+2,s+2,l);
	return mem;		
}

