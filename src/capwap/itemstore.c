
#include <stdlib.h>
#include <string.h>

#include "itemstore.h"



static inline void cw_itemstore_del_data(void *e)
{
	struct cw_item * item = (struct cw_item*)e;
	if (item->type==CW_ITEMTYPE_DATA)
		free (item->data);
	
}

static void cw_itemstore_del(void * e)
{

	cw_itemstore_del_data(e);
	free(e);	
}


static int cw_itemstore_cmp(const void *x1,const void*x2)
{
	return ((struct cw_item*)x1)->id - ((struct cw_item*)x2)->id;
}


cw_itemstore_t cw_itemstore_create()
{
	return avltree_create(cw_itemstore_cmp,cw_itemstore_del);
}


struct cw_item * cw_item_create(cw_itemstore_t s,uint32_t id)
{
	struct cw_item is;
	is.id=id;

	struct cw_item *i=avltree_get(s,&is);
	if (i) {
		cw_itemstore_del_data(i);
		return i;
	}

	i = malloc(sizeof(struct cw_item));
	if (!i)
		return 0;
	i->id=id;
	return avltree_add(s,i);
}



int cw_itemstore_set_byte(cw_itemstore_t s,uint32_t id,uint8_t byte)
{
	struct cw_item *i = cw_item_create(s,id);
	if (!i)
		return 0;
	i->byte=byte;
	i->type=CW_ITEMTYPE_BYTE;
	return 1;
}

int cw_itemstore_set_word(cw_itemstore_t s,uint32_t id,uint32_t word)
{
	struct cw_item *i = cw_item_create(s,id);
	if (!i)
		return 0;
	i->word=word;
	i->type=CW_ITEMTYPE_WORD;
	return 1;
}

int cw_itemstore_set_dword(cw_itemstore_t s,uint32_t id,uint32_t dword)
{
	struct cw_item *i = cw_item_create(s,id);
	if (!i)
		return 0;
	i->dword=dword;
	i->type=CW_ITEMTYPE_DWORD;
	return 1;
}

int cw_itemstore_set_str(cw_itemstore_t s,uint32_t id,char *str)
{
	struct cw_item *i = cw_item_create(s,id);
	if (!i)
		return 0;
	i->type=CW_ITEMTYPE_DATA;
	i->data=strdup(str);
	return 1;
}

int cw_itemstore_set_strn(cw_itemstore_t s,uint32_t id,const char *str,int n)
{
	struct cw_item *i = cw_item_create(s,id);
	if (!i)
		return 0;
	i->type=CW_ITEMTYPE_DATA;
	i->data=strndup(str,n);
	return 1;
}
		

/*
	struct cw_item * i = malloc(sizeof(struct cw_item));
	if ( !i)
		return 0;
	i->id=id;
	i->type=CW_ITEMTYPE_BYTE;
	i->byte=byte;
	struct cw_item * r = avltree_add(s,i);
	if (r!=i) {
		r->byte=byte;
		free(i);
	}
	return 1;	
*/


/*
char * cw_itemstore_get_str(cw_itemstore_t s,uint32_t id)
{
//	struct cw_item * item = avltree_get(s,id);

}
*/

//cw_itemstore_set_str(

//	cw_item_t item = (cw_item_t) avltree_get(t,a);

