
#include "mbag.h"
#include "format.h"



static int to_str(void *item,char *dst)
{
	mbag_item_t *i= item;

	char *d = dst;
	int utf8 = format_is_utf8(bstr16_data(i->data), bstr16_len(i->data));


        if (utf8) {
                d += sprintf(d, "%.*s", bstr16_len(i->data), bstr16_data(i->data));
        } else {
                d += sprintf(d, ".x");
                d += format_hex(d, bstr16_data(i->data), bstr16_len(i->data));
        }
	return d-dst;	
}


static struct mbag_item *  from_str(const char *src)
{
	mbag_item_t * i = mbag_item_new(MBAG_BSTR16);
	if (!i)
		return NULL;
	i->data=bstr16_create_from_str(src);

	return i;
}

int put(struct mbag_item *i, uint8_t *dst)
{
	int l = bstr16_len(i->data);
	memcpy(dst,bstr16_data(i->data),l);
	return l;
}

static struct mbag_item * get(const uint8_t *src, int len)
{
	mbag_item_t * i = mbag_item_new(MBAG_BSTR16);
	if (!i)
		return NULL;
	i->data=bstr16_create(src,len);
	return i;

}

const struct mbag_typedef mbag_type_bstr16 = {
	.name = "Bstr16",
	.del = free,
	.to_str = to_str,
	.from_str = from_str,
	.get = get,
	.put = put
};
