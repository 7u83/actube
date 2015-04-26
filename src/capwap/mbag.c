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

const struct mbag_typedef mbag_type_const_data = {
	NULL
};

const struct mbag_typedef mbag_type_bstr = {
	free
};

const struct mbag_typedef mbag_type_bstr16 = {
	free
};

const struct mbag_typedef mbag_type_str = {
	free
};

const struct mbag_typedef mbag_type_vendorstr = {
	free
};


void free_avltree(void*t){
	mavl_t i = (mavl_t) t;

	mavl_destroy(i);

}
const struct mbag_typedef mbag_type_avltree = {
	free_avltree
};




static void mbag_type_mbag_del(void *i)
{
	mbag_t x = (mbag_t)i;
	
	mavl_destroy(x);
}

const struct mbag_typedef mbag_type_mbag = {
	mbag_type_mbag_del
};


static void mbag_del_data(struct mbag_item *i)
{
	if (i->type->del) {
		i->type->del(i->data);
	}
}


static void mbag_del_fun(void *e)
{

	mbag_del_data(e);
	free(e);
}


static void strmbag_del_fun(void *e)
{

	mbag_del_data(e);
	free(e);
}


static int strmbag_cmp_fun(const void *x1, const void *x2)
{
	return strcmp(( (struct mbag_item *) x1)->name , ((struct mbag_item *) x2)->name  );
}



static int mbag_cmp_fun(const void *x1, const void *x2)
{
	return ((struct mbag_item *) x1)->id - ((struct mbag_item *) x2)->id;
}

mbag_t mbag_create()
{
	return mavl_create(mbag_cmp_fun, mbag_del_fun);
}


mbag_t strmbag_create()
{
	return mavl_create(strmbag_cmp_fun, strmbag_del_fun);

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


struct mbag_item *strmbag_item_create(mbag_t s, char *name)
{
	struct mbag_item is;
	is.name = name;


	struct mbag_item *i = mavl_get(s, &is);
	if (i) {
		mbag_del_data(i);
		return i;
	}

	i = malloc(sizeof(struct mbag_item));
	if (!i)
		return NULL;
	i->name = name;
	return mavl_add(s, i);
}


