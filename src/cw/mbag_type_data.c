
#include <string.h>

#include "mbag.h"
#include "format.h"



static int to_str(void *item,char *dst)
{
	mbag_item_t *i= item;
	return format_hex(dst, ((uint8_t*)i->data)+1, *((uint8_t*)i->data));
}


static struct mbag_item *  from_str(const char *src)
{

	mbag_item_t * i = mbag_item_new(MBAG_DATA);
	if (!i)
		return NULL;


	int l=strlen(src);
	int msize=l/2;	
	if(l&1)
		msize++;
	uint8_t * mem = malloc(1+msize);
	if (!mem)
		return NULL;

	*(mem)=msize;
	format_scan_hex_bytes(mem+1,src,l);
	i->data=mem;
	return i;		
	
}



const struct mbag_typedef mbag_type_data = {
	"Data",free,to_str,from_str
};
