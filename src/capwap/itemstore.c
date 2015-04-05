
#include <stdlib.h>
#include <string.h>

#include "itemstore.h"



static inline void cw_itemstore_del_data(void *e)
{
	struct cw_item *item = (struct cw_item *) e;
	switch (item->type) {
		case CW_ITEMTYPE_DATA:
		case CW_ITEMTYPE_STR:
		case CW_ITEMTYPE_BSTR:
		case CW_ITEMTYPE_VERSION:
		case CW_ITEMTYPE_FUN:
			free(item->data);
			break;
		case CW_ITEMTYPE_AVLTREE:
			avltree_destroy(item->data);


	}
}

static void cw_itemstore_del(void *e)
{

	cw_itemstore_del_data(e);
	free(e);
}


static int cw_itemstore_cmp(const void *x1, const void *x2)
{
	return ((struct cw_item *) x1)->id - ((struct cw_item *) x2)->id;
}


cw_itemstore_t cw_itemstore_create()
{
	return avltree_create(cw_itemstore_cmp, cw_itemstore_del);
}


struct cw_item *cw_item_create(cw_itemstore_t s, uint32_t id)
{
	struct cw_item is;
	is.id = id;

	struct cw_item *i = avltree_get(s, &is);
	if (i) {
		cw_itemstore_del_data(i);
		return i;
	}

	i = malloc(sizeof(struct cw_item));
	if (!i)
		return 0;
	i->id = id;
	return avltree_add(s, i);
}



int cw_itemstore_set_byte(cw_itemstore_t s, uint32_t id, uint8_t byte)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;
	i->byte = byte;
	i->type = CW_ITEMTYPE_BYTE;
	return 1;
}

int cw_itemstore_set_word(cw_itemstore_t s, uint32_t id, uint32_t word)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;
	i->word = word;
	i->type = CW_ITEMTYPE_WORD;
	return 1;
}

int cw_itemstore_set_dword(cw_itemstore_t s, uint32_t id, uint32_t dword)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;
	i->dword = dword;
	i->type = CW_ITEMTYPE_DWORD;
	return 1;
}

int cw_itemstore_set_str(cw_itemstore_t s, uint32_t id, const char *str)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;
	i->type = CW_ITEMTYPE_STR;
	i->data = strdup(str);
	return 1;
}

int cw_itemstore_set_strn(cw_itemstore_t s, uint32_t id, const char *str, int n)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;
	i->type = CW_ITEMTYPE_DATA;
	i->data = strndup(str, n);
	return 1;
}

int cw_itemstore_set_ptr(cw_itemstore_t s, uint32_t id, void *ptr)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;
	i->type = CW_ITEMTYPE_DATA;
	i->data = ptr;
	return 1;
}

int cw_itemstore_set_bstrn(cw_itemstore_t s, uint32_t id, uint8_t * data, int len)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;
	i->type = CW_ITEMTYPE_BSTR;
	i->data = bstr_create(data, len);
	return 1;
}


int cw_itemstore_set_const_ptr(cw_itemstore_t s, uint32_t id, void *ptr)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;
	i->type = CW_ITEMTYPE_CONST_DATA;
	i->data = ptr;
	return 1;
}

int cw_itemstore_set_version(cw_itemstore_t s, uint32_t id, uint32_t vendor_id,
			     uint8_t * versionstr, int len)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;
	i->type = CW_ITEMTYPE_VERSION;

	uint8_t *ptr = malloc(bstr16_size(len) + 4);
	i->data = ptr;

	if (!ptr)
		return 1;

	*((uint32_t *) ptr) = vendor_id;
	bstr16_ncpy(ptr + 4, versionstr, len);

	return 1;
}

int cw_itemstore_set_avltree(cw_itemstore_t s, uint32_t id, struct avltree *t)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;
	i->type = CW_ITEMTYPE_AVLTREE;
	i->data = t;
	return 1;
}



struct cw_item_fundef {
	void *(*get) (void *arg);
	void (*free) (void *arg, void *data);
	void *arg;
};

int cw_itemstore_set_fun(cw_itemstore_t s, uint32_t id,
			 void *(*funget) (void *arg),
			 void (*funfree) (void *arg, void *data), void *arg)
{
	struct cw_item *i = cw_item_create(s, id);
	if (!i)
		return 0;

	struct cw_item_fundef *fundef = malloc(sizeof(struct cw_item_fundef));
	i->data = fundef;
	if (!fundef)
		return 0;

	fundef->get = funget;
	fundef->free = funfree;
	fundef->arg = arg;
	i->type=CW_ITEMTYPE_FUN;

	return 1;
}

void *cw_item_get_data_ptr(struct cw_item *item)
{
	switch (item->type) {
		case CW_ITEMTYPE_FUN:
			{
				struct cw_item_fundef *fundef =
				    (struct cw_item_fundef *) item->data;
				if (!fundef)
					return NULL;
				return fundef->get(fundef->arg);
			}

	}
	return item->data;

}


void cw_item_release_data_ptr(struct cw_item *item, void *data)
{
	switch (item->type) {
		case CW_ITEMTYPE_FUN:
			{
				struct cw_item_fundef *fundef =
				    (struct cw_item_fundef *) item->data;
				if (!fundef)
					return;
				if (!fundef->free)
					return;

				return fundef->free(fundef->arg, data);
			}

	}
}
