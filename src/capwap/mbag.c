#include "mbag.h"


const struct mbag_typedef mbag_type_byte = {
	NULL
};

const struct mbag_typedef mbag_type_word = {
	NULL
};

const struct mbag_typedef mbag_type_dword = {
	NULL
};

const struct mbag_typedef mbag_type_bstr = {
	free
};

const struct mbag_typedef mbag_type_bstr16 = {
	free
};



static void mbag_type_mbag_del(void *i)
{
	
	mavl_destroy(((struct mbag_item*)i)->data);
}

const struct mbag_typedef mbag_type_mbag = {
	mbag_type_mbag_del
};


static void mbag_del_data(struct mbag_item *i)
{
	if (i->type->del)
		i->type->del(i);
}


static void mbag_del_fun(void *e)
{
	mbag_del_data(e);
	free(e);
}


static int mbag_cmp_fun(const void *x1, const void *x2)
{
	return ((struct mbag_item *) x1)->id - ((struct mbag_item *) x2)->id;
}

mbag_t mbag_create()
{
	return mavl_create(mbag_cmp_fun, mbag_del_fun);
}

struct mbag_item *mbag_item_create(mbag_t s, uint32_t id)
{
	struct mbag_item is;
	is.id = id;

	struct mbag_item *i = mavl_get(s, &is);
	if (i) {
		mbag_del_data(i);
		return i;
	}

	i = malloc(sizeof(struct mbag_item));
	if (!i)
		return NULL;
	i->id = id;
	return mavl_add(s, i);
}




