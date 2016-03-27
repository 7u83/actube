#include "mbag.h"

static int to_str(void *item,char *dst)
{
	mbag_item_t *i= item;
        return sprintf(dst, "%s", (char*)i->data);
}

static struct mbag_item *  from_str(const char *src)
{
	struct mbag_item *i= malloc(sizeof(mbag_item_t));
	if (!i)
		return NULL;

	i->type = MBAG_STR;
	i->dynid=0;
	i->data = strndup(src,2000);
	return i;
}


const struct mbag_typedef mbag_type_str = {
	"Sstr",free,to_str,from_str
};


