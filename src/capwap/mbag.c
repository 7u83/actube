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
	mbag_item_t *i = (mbag_item_t *) t;

printf("the mavl deleer %p\n",i->data);
	mavl_destroy(i->data);
printf("jo\n");

}
const struct mbag_typedef mbag_type_avltree = {
	free_avltree
};




static void mbag_type_mbag_del(void *i)
{
	struct mbag_item *x = (struct mbag_item*)i;
printf("MBAG DELETE %p\n",x->data);
	
	mavl_destroy(x->data);
}

const struct mbag_typedef mbag_type_mbag = {
	mbag_type_mbag_del
};


static void mbag_del_data(struct mbag_item *i)
{
printf("Deling: %p\n",i->data);
	if (i->type->del) {
printf("Calling deler\n");
		i->type->del(i);
	}
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




