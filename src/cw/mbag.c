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

/*const struct mbag_typedef mbag_type_mbag = {
#	"mbag",mbag_type_mbag_del
};
*/

/*const struct mbag_typedef mbag_type_mbag_dyn = {
	"mbag",mbag_type_mbag_del
};
*/

static void mbag_del_data(struct mbag_item *i)
{
	if (i->type->del) {
		i->type->del(i->u2.data);
	}
}


static void mbag_del_fun(void *e)
{
	mbag_item_t * i = (mbag_item_t*)e;
	if (i->dynid){
		free((void*)i->u1.id);
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
	return strcmp(( (struct mbag_item *) x1)->u1.id , ((struct mbag_item *) x2)->u1.id  );
}



static int mbag_i_cmp_fun(const void *x1, const void *x2)
{
	return ((struct mbag_item *) x1)->u1.iid - ((struct mbag_item *) x2)->u1.iid;
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
	is.u1.id = id;


	struct mbag_item *i = mavl_get(s, &is);
	if (i) {
		mbag_del_data(i);
		return i;
	}

	i = malloc(sizeof(struct mbag_item));
	if (!i)
		return NULL;
	i->u1.id = id;
	i->dynid=0;
	return mavl_add(s, i);
}


struct mbag_item *mbag_i_item_create(mbag_t s, uint32_t id)
{
	struct mbag_item is;
	is.u1.iid = id;


	struct mbag_item *i = mavl_get(s, &is);
	if (i) {
		mbag_del_data(i);
		return i;
	}

	i = malloc(sizeof(struct mbag_item));
	if (!i)
		return NULL;
	i->u1.iid = id;
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













int mbag_set(mbag_t t,mbag_item_t *is)
{
	mavl_del(t,is);
	mavl_add(t,is);
	return 1;

/*
	struct mbag_item *i = mavl_get(s, is);
	if (i) {
		mbag_del_data(i);
		return i;
	}
	mavl_add(t,is);
*/
}

int mbag_set_byte(mbag_t s, const char *id, uint8_t byte)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->u2.byte = byte;
	i->type = &mbag_type_byte;
	return 1;
}

struct mbag_item * mbag_set_word(mbag_t s, const char *id, uint16_t word)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->u2.word = word;
	i->type = &mbag_type_word;
	return i;
}




struct mbag_item * mbag_set_dword(mbag_t s, const char *id, uint32_t dword)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return NULL;
	i->u2.dword = dword;
	i->type = &mtype_dword;
	return i;
}


int mbag_i_set_mbag(mbag_t s, uint32_t id, mbag_t mbag)
{
	struct mbag_item *i = mbag_i_item_create(s, id);
	if (!i)
		return 0;
	i->u2.data = mbag;
	i->type = &mbag_type_mbag;
	return 1;
}

int mbag_set_mbag(mbag_t s, const char *id, mbag_t mbag)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->u2.data = mbag;
	i->type = &mbag_type_mbag;
	return 1;
}

int mbag_set_data(mbag_t s, const char *id, const struct mbag_typedef *type, void *data)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->u2.data = data;
	i->type = type;
	return 1;


}


bstrv_t mbag_set_bstrv(mbag_t s, const char *id, uint32_t vendor_id,
			     uint8_t * version, int len)
{
	mbag_item_t *i = mbag_item_create(s, id);
	if (!i)
		return NULL;
	
	i->type = MBAG_VENDORSTR;
	i->u2.data = bstrv_create(vendor_id,version,len);
	return i->u2.data;
}













mbag_item_t *mbag_get(mbag_t s, const char *id)
{
	mbag_item_t i;
	i.u1.id = id;
	i.type=0;
	return mavl_get(s, &i);
}


mbag_item_t *mbag_i_get(mbag_t s, uint32_t id)
{
	mbag_item_t i;
	i.u1.iid = id;
	i.type=0;
	return mavl_get(s, &i);
}



void mbag_del(mbag_t s,const char *id)
{
	mbag_item_t i;
	i.u1.id = id;
	i.type=0;
	mavl_del(s, &i);
}

void * mbag_get_data(mbag_t s, const char *id, const struct mbag_typedef * type, void * def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != type)
		return def;
	return i->u2.data;
}






















/***************************************************/


void * mbag_get_raw(mbag_t s, const char *id, void * def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	return i->u2.data;
}



uint8_t mbag_get_byte(mbag_t s, const char *id, uint8_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_BYTE)
		return def;
	return i->u2.byte;
}

uint16_t mbag_get_word(mbag_t s, const char *id, uint16_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_WORD)
		return def;
	return i->u2.word;
}



uint32_t mbag_get_dword(mbag_t s, const char *id, uint32_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MTYPE_DWORD)
		return def;
	return i->u2.dword;
}


mbag_t mbag_i_get_mbag(mbag_t s, uint32_t id, mbag_t def)
{
	struct mbag_item *i = mbag_i_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_MBAG)
		return def;
	return i->u2.data;
}


mbag_t mbag_get_mbag(mbag_t s, const char *id, mbag_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_MBAG)
		return def;
	return i->u2.data;
}

mbag_item_t * mbag_set_bstrn(mbag_t s, const char *id, uint8_t * data, int len)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return NULL;
	i->type = MBAG_BSTR;
	i->u2.data = bstr_create(data, len);
	return i;
}




struct mavl *mbag_get_mavl(mbag_t s, const char *id)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return NULL;
	if (i->type != MBAG_AVLTREE)
		return NULL;
	return i->u2.data;
}

int mbag_i_set_mavl(mbag_t s, uint32_t id, mavl_t t)
{
	struct mbag_item *i = mbag_i_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_AVLTREE;
	i->u2.data = t;
	return 1;
}

int mbag_set_mavl(mbag_t s, const char *id, mavl_t t)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_AVLTREE;
	i->u2.data = t;
	return 1;
}

/*
struct mavl *mbag_get_avltree_c(mbag_t s, const char *id,
							 struct mavl *(creator) ())
{
	struct mavl * avltree = mbag_get_mavl(s, id);
	if (avltree){
		return avltree;
	}

	avltree = creator();
	if (!avltree)
		return NULL;
	mbag_set_mavl(s, id, avltree);
	return avltree;
}
*/

mavl_t mbag_get_mavl_c(mbag_t s, const char *id,
							 mavl_t (creator) ())
{
	struct mbag_item *i = mbag_get(s, id);
	if (i)
		return i->u2.data;

	if (!creator)
		return NULL;
	mavl_t avltree = creator();
	if (!avltree)
		return NULL;
	mbag_set_mavl(s, id, avltree);
	return avltree;
}


mavl_t mbag_get_mbag_c(mbag_t s, const char *id,
							 mavl_t (creator) ())
{
	struct mbag_item *i = mbag_get(s, id);
	if (i)
		return i->u2.data;

	if (!creator)
		return NULL;
	mavl_t avltree = creator();
	if (!avltree)
		return NULL;
	mbag_set_mbag(s, id, avltree);
	return avltree;
}



int mbag_inc_word(mbag_t s, const char * id,int n)
{
	uint16_t w = mbag_get_word(s,id,0);
	mbag_set_word(s,id,w+n);
	return 1;
}



mavl_t mbag_i_get_mbag_c(mbag_t s, uint32_t iid,
							 mavl_t (creator) ())
{
	struct mbag_item *i = mbag_i_get(s, iid);
	if (i)
		return i->u2.data;

	if (!creator)
		return NULL;
	mavl_t avltree = creator();
	if (!avltree)
		return NULL;
	mbag_i_set_mbag(s, iid, avltree);
	return avltree;
}




mavl_t mbag_i_get_mavl(mbag_t s, uint32_t id,
							 mavl_t (creator) ())
{
	struct mbag_item *i = mbag_i_get(s, id);
	if (i)
		return i->u2.data;

	if (!creator)
		return NULL;
	mavl_t avltree = creator();
	if (!avltree)
		return NULL;
	mbag_i_set_mavl(s, id, avltree);
	return avltree;
}





















char *mbag_get_str(mbag_t s, const char *id, char *def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	return i->u2.data;

};

int mbag_set_str(mbag_t s, const char *id, const char *str)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_STR;
	i->u2.data = strdup(str);
	return 1;
}


int mbag_set_sockaddr(mbag_t s, const char *id, const struct sockaddr * sa)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	struct sockaddr_storage * sad = malloc(sizeof(struct sockaddr_storage));
	if (!sad){
		free(i);
		return 0;
	}
	sock_copyaddr(sad,sa);
	i->type = MBAG_SOCKADDR;
	i->u2.data = sad;
	return 1;

}




























/*******/

void *mbag_item_get_data_ptr(struct mbag_item *item)
{
	if (item->type == MBAG_FUN ) 
	{
				struct mbag_item_fundef *fundef =
				    (struct mbag_item_fundef *) item->u2.data;
				if (!fundef)
					return NULL;
				return fundef->get(fundef->arg);
	}
	return item->u2.data;
}


void mbag_item_release_data_ptr(struct mbag_item *item, void *data)
{
	if (item->type ==  MBAG_FUN)
			{
				struct mbag_item_fundef *fundef =
				    (struct mbag_item_fundef *) item->u2.data;
				if (!fundef)
					return;
				if (!fundef->free)
					return;

				return fundef->free(fundef->arg, data);
			}

}

int mbag_set_strn(mbag_t s, const char *id, const char *str, int n)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_STR;
	i->u2.data = strndup(str, n);
	return 1;
}

int mbag_set_bstr16n(mbag_t s, const char *id, uint8_t * data, int len)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_BSTR16;
	i->u2.data = bstr16_create(data, len);
	return 1;
}



mbag_item_t * mbag_set_const_ptr(mbag_t s, const char *id, void *ptr)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_CONST_DATA;
	i->u2.data = ptr;
	return i;
}

 
int mbag_set_ptr(mbag_t s, const char *id, void *ptr)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_PTR;
	i->u2.data = ptr;
	return 1;
}



int mbag_set_fun(mbag_t s, const char *id,
			 void *(*funget) (void *arg),
			 void (*funfree) (void *arg, void *data), void *arg)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;

	struct mbag_item_fundef *fundef = malloc(sizeof(struct mbag_item_fundef));
	i->u2.data = fundef;
	if (!fundef)
		return 0;

	fundef->get = funget;
	fundef->free = funfree;
	fundef->arg = arg;
	i->type=MBAG_FUN;

	return 1;
}







