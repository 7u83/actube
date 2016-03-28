
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

const struct mbag_typedef mbag_type_bstr16 = {
	"Bstr16",free,to_str,from_str
};
