#include "mbag.h"
#include "capwap.h"
#include "format.h"
/**
 * @file 
 * @MBAG functions
 * @addtogroup MbagFunctions
 * @{
 */ 

/*
static int mbag_bstr16str(void *item,char *dst)
{
	mbag_item_t *i= item;



	char *d = dst;
	int utf8 = cw_is_utf8(bstr16_data(i->data), bstr16_len(i->data));


        if (utf8) {
                d += sprintf(d, "%.*s", bstr16_len(i->data), bstr16_data(i->data));
        } else {
                d += sprintf(d, "\".x");
                d += cw_format_hex(d, bstr16_data(i->data), bstr16_len(i->data));
                d += sprintf(d, "\"");
        }
	return d-dst;	
}
*/


/*
const struct mbag_typedef mbag_type_byte = {
	"byte",NULL
};
*/

/*
const struct mbag_typedef mbag_type_word = {
	"word",NULL
};
*/

/*
const struct mbag_typedef mbag_type_dword = {
	"dword",NULL
};
*/

const struct mbag_typedef mbag_type_const_data = {
	"data",NULL
};

const struct mbag_typedef mbag_type_bstr = {
	"bstr",free
};

/*
const struct mbag_typedef mbag_type_bstr16 = {
	"bstr16",free,mbag_bstr16str
};

*/




/*
const struct mbag_typedef mbag_type_vendorstr = {
	"vendorstr",free
};
*/


void free_avltree(void*t){
	mavl_t i = (mavl_t) t;

	mavl_destroy(i);

}
const struct mbag_typedef mbag_type_avltree = {
	"mavl",free_avltree
};




static void mbag_type_mbag_del(void *i)
{
	mbag_t x = (mbag_t)i;
	
	mavl_destroy(x);
}

const struct mbag_typedef mbag_type_mbag = {
	"mbag",mbag_type_mbag_del
};

const struct mbag_typedef mbag_type_mbag_dyn = {
	"mbag",mbag_type_mbag_del
};


static void mbag_del_data(struct mbag_item *i)
{
	if (i->type->del) {
		i->type->del(i->data);
	}
}


static void mbag_del_fun(void *e)
{
	mbag_item_t * i = (mbag_item_t*)e;
	if (i->dynid){
		free((void*)i->id);
	}
	
	mbag_del_data(e);
	free(e);
}


static void mbag_i_del_fun(void *e)
{

	mbag_del_data(e);
	free(e);
}


static int mbag_cmp_fun(const void *x1, const void *x2)
{
	return strcmp(( (struct mbag_item *) x1)->id , ((struct mbag_item *) x2)->id  );
}



static int mbag_i_cmp_fun(const void *x1, const void *x2)
{
	return ((struct mbag_item *) x1)->iid - ((struct mbag_item *) x2)->iid;
}

mbag_t mbag_i_create()
{
	return mavl_create(mbag_i_cmp_fun, mbag_i_del_fun);
}


mbag_t mbag_create()
{
	return mavl_create(mbag_cmp_fun, mbag_del_fun);

}



mbag_item_t *mbag_item_new(mbagtype_t type)
{
	struct mbag_item *i= malloc(sizeof(mbag_item_t));
	if (!i)
		return NULL;

	i->type = type;
	i->dynid=0;
	return i;
}

struct mbag_item *mbag_item_create(mbag_t s, const char *id)
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
	i->dynid=0;
	return mavl_add(s, i);
}


struct mbag_item *mbag_i_item_create(mbag_t s, uint32_t id)
{
	struct mbag_item is;
	is.iid = id;


	struct mbag_item *i = mavl_get(s, &is);
	if (i) {
		mbag_del_data(i);
		return i;
	}

	i = malloc(sizeof(struct mbag_item));
	if (!i)
		return NULL;
	i->iid = id;
	i->dynid=0;
	return mavl_add(s, i);
}





/*
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

*/

/**@}*/
