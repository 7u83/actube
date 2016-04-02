
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

static struct mbag_item *  get(const uint8_t *src,int len)
{

	mbag_item_t * item = mbag_item_new(MBAG_DATA);
	if (!item)
		return NULL;

	uint8_t *data = malloc(len+1);
	if (!data){
		free (item);
		return NULL;
	}
	*data=len;
	memcpy(data+1,src,len);
	item->data=data;
	return item;
}




const struct mbag_typedef mbag_type_data = {
	.name = "Binary Data",
	.del = free,
	.from_str = from_str,
	.to_str = to_str,
	.get = get

};
