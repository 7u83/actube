/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef __MBAG_H
#define __MBAG_H

/** 
 * @defgroup MBaglFunctions MBAG Functions
 * @{
 */


#include <stdint.h>
#include "mavl.h"
#include "bstr.h"


struct mbag_item;

/** 
 * Definition for MABG data types 
 */ 
struct mbag_typedef{
	/** A human readable name for this type */
	const char *name;
	/** A pointer to a function to delete elements of this type */
	void (*del)(void*);
	/** A pointer to convert elements of this type to a string.
	    This function is maily used to store elements to a SQL database
	    or to json strings */
	int (*to_str)(void*,char *dst);
	/** Cereate an item of this type from a string, which was previously
	    created by the #del function. */
	struct mbag_item * (*from_str)(const char *src);
};

/** The type for an mbag typedf */
typedef const struct mbag_typedef * mbagtype_t;



/**
 * The MBAG item 
 */ 
struct mbag_item{
	/** 
	 * Key of this item 
	 * This could be either a string (default) or an integer uint32_t value.
	 * If you whant to use with uint32_t keys, use the mbag_i_* functions, to 
	 * create mbags. 
	 */
	union {
		uint32_t iid;
		const char *id;
	};
	/** Type of this item */
	const struct mbag_typedef * type;
	/** Value of this item */
	union {
		void *data;
		uint8_t byte;
		uint16_t word;
		uint32_t dword;
	};
};


typedef struct mbag_item mbag_item_t;

/** 
 * The MBAG Type
 * It's simply an #mavl AVL Tree.
 */ 
typedef mavl_t mbag_t;


extern const struct mbag_typedef mbag_type_byte;
extern const struct mbag_typedef mbag_type_word;
extern const struct mbag_typedef mbag_type_dword;
extern const struct mbag_typedef mbag_type_mbag;
extern const struct mbag_typedef mbag_type_bstr;
extern const struct mbag_typedef mbag_type_bstr16;
extern const struct mbag_typedef mbag_type_vendorstr;
extern const struct mbag_typedef mbag_type_str;
extern const struct mbag_typedef mbag_type_avltree;
extern const struct mbag_typedef mbag_type_const_data;

#define MBAG_BYTE (&mbag_type_byte)
#define MBAG_WORD (&mbag_type_word)
#define MBAG_DWORD (&mbag_type_dword)
#define MBAG_MBAG (&mbag_type_mbag)
#define MBAG_BSTR (&mbag_type_bstr)
#define MBAG_BSTR16 (&mbag_type_bstr16)
#define MBAG_VENDORSTR (&mbag_type_vendorstr)
#define MBAG_STR (&mbag_type_str)
#define MBAG_DATA MBAG_STR
#define MBAG_AVLTREE (&mbag_type_avltree)
#define MBAG_FUN MBAG_STR
#define MBAG_CONST_DATA (&mbag_type_const_data)


extern mbag_t mbag_create();
extern mbag_t mbag_i_create();

struct mbag_item *mbag_item_create(mbag_t s, const char *id);
struct mbag_item *mbag_i_item_create(mbag_t s, uint32_t id);


static inline int mbag_set(mbag_t t,mbag_item_t *is)
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

static inline int mbag_set_byte(mbag_t s, const char *id, uint8_t byte)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->byte = byte;
	i->type = &mbag_type_byte;
	return 1;
}

static inline int mbag_set_word(mbag_t s, const char *id, uint16_t word)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->word = word;
	i->type = &mbag_type_word;
	return 1;
}



static inline int mbag_set_dword(mbag_t s, const char *id, uint32_t dword)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->dword = dword;
	i->type = &mbag_type_dword;
	return 1;
}


static inline int mbag_i_set_mbag(mbag_t s, uint32_t id, mbag_t mbag)
{
	struct mbag_item *i = mbag_i_item_create(s, id);
	if (!i)
		return 0;
	i->data = mbag;
	i->type = &mbag_type_mbag;
	return 1;
}

static inline int mbag_set_mbag(mbag_t s, const char *id, mbag_t mbag)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->data = mbag;
	i->type = &mbag_type_mbag;
	return 1;
}

static inline int mbag_set_data(mbag_t s, const char *id, const struct mbag_typedef *type, void *data)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->data = data;
	i->type = type;
	return 1;


}


static inline vendorstr_t mbag_set_vendorstr(mbag_t s, const char *id, uint32_t vendor_id,
			     uint8_t * vendorstr, int len)
{
	mbag_item_t *i = mbag_item_create(s, id);
	if (!i)
		return NULL;
	
	i->type = MBAG_VENDORSTR;
	i->data = vendorstr_create(vendor_id,vendorstr,len);
	return i->data;
}





#define mbag_set_bstr(m,i,d) mbag_set_data(m,i,MBAG_BSTR,d)
#define mbag_set_bstr16(m,i,d) mbag_set_data(m,i,MBAG_BSTR16,d)


static inline mbag_item_t *mbag_get(mbag_t s, const char *id)
{
	mbag_item_t i;
	i.id = id;
	i.type=0;
	return mavl_get(s, &i);
}


static inline mbag_item_t *mbag_i_get(mbag_t s, uint32_t id)
{
	mbag_item_t i;
	i.iid = id;
	i.type=0;
	return mavl_get(s, &i);
}



static inline void mbag_del(mbag_t s,const char *id)
{
	mbag_item_t i;
	i.id = id;
	i.type=0;
	mavl_del(s, &i);
}

static inline void * mbag_get_data(mbag_t s, const char *id, const struct mbag_typedef * type, void * def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != type)
		return def;
	return i->data;
}

#define mbag_get_bstr(m,i,d) mbag_get_data(m,i,MBAG_BSTR,d)
#define mbag_get_bstr16(m,i,d) mbag_get_data(m,i,MBAG_BSTR16,d)

static inline void * mbag_get_raw(mbag_t s, const char *id, void * def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	return i->data;
}



static inline uint8_t mbag_get_byte(mbag_t s, const char *id, uint8_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_BYTE)
		return def;
	return i->byte;
}

static inline uint16_t mbag_get_word(mbag_t s, const char *id, uint16_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_WORD)
		return def;
	return i->word;
}



static inline uint32_t mbag_get_dword(mbag_t s, const char *id, uint32_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_DWORD)
		return def;
	return i->dword;
}


static inline mbag_t mbag_i_get_mbag(mbag_t s, uint32_t id, mbag_t def)
{
	struct mbag_item *i = mbag_i_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_MBAG)
		return def;
	return i->data;
}


static inline mbag_t mbag_get_mbag(mbag_t s, const char *id, mbag_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_MBAG)
		return def;
	return i->data;
}

static inline mbag_item_t * mbag_set_bstrn(mbag_t s, const char *id, uint8_t * data, int len)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return NULL;
	i->type = MBAG_BSTR;
	i->data = bstr_create(data, len);
	return i;
}

static inline struct mavl *mbag_get_mavl(mbag_t s, const char *id)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return NULL;
	if (i->type != MBAG_AVLTREE)
		return NULL;
	return i->data;
}

static inline int mbag_i_set_mavl(mbag_t s, uint32_t id, mavl_t t)
{
	struct mbag_item *i = mbag_i_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_AVLTREE;
	i->data = t;
	return 1;
}

static inline int mbag_set_mavl(mbag_t s, const char *id, mavl_t t)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_AVLTREE;
	i->data = t;
	return 1;
}

/*
static inline struct mavl *mbag_get_avltree_c(mbag_t s, const char *id,
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

static inline mavl_t mbag_get_mavl_c(mbag_t s, const char *id,
							 mavl_t (creator) ())
{
	struct mbag_item *i = mbag_get(s, id);
	if (i)
		return i->data;

	if (!creator)
		return NULL;
	mavl_t avltree = creator();
	if (!avltree)
		return NULL;
	mbag_set_mavl(s, id, avltree);
	return avltree;
}

static inline mavl_t mbag_i_get_mavl(mbag_t s, uint32_t id,
							 mavl_t (creator) ())
{
	struct mbag_item *i = mbag_i_get(s, id);
	if (i)
		return i->data;

	if (!creator)
		return NULL;
	mavl_t avltree = creator();
	if (!avltree)
		return NULL;
	mbag_i_set_mavl(s, id, avltree);
	return avltree;
}




/** @} */

static inline char *mbag_get_str(mbag_t s, const char *id, char *def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	return i->data;

};

static inline int mbag_set_str(mbag_t s, const char *id, const char *str)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_STR;
	i->data = strdup(str);
	return 1;
}



struct mbag_item_fundef {
	void *(*get) (void *arg);
	void (*free) (void *arg, void *data);
	void *arg;
};


static inline void *mbag_item_get_data_ptr(struct mbag_item *item)
{
	if (item->type == MBAG_FUN ) 
	{
				struct mbag_item_fundef *fundef =
				    (struct mbag_item_fundef *) item->data;
				if (!fundef)
					return NULL;
				return fundef->get(fundef->arg);
	}
	return item->data;
}


static inline void mbag_item_release_data_ptr(struct mbag_item *item, void *data)
{
	if (item->type ==  MBAG_FUN)
			{
				struct mbag_item_fundef *fundef =
				    (struct mbag_item_fundef *) item->data;
				if (!fundef)
					return;
				if (!fundef->free)
					return;

				return fundef->free(fundef->arg, data);
			}

}

static inline int mbag_set_strn(mbag_t s, const char *id, const char *str, int n)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_STR;
	i->data = strndup(str, n);
	return 1;
}

static inline int mbag_set_bstr16n(mbag_t s, const char *id, uint8_t * data, int len)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_BSTR16;
	i->data = bstr16_create(data, len);
	return 1;
}



static inline mbag_item_t * mbag_set_const_ptr(mbag_t s, const char *id, void *ptr)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_CONST_DATA;
	i->data = ptr;
	return i;
}


static inline int mbag_set_ptr(mbag_t s, const char *id, void *ptr)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->type = MBAG_DATA;
	i->data = ptr;
	return 1;
}




static inline int mbag_set_fun(mbag_t s, const char *id,
			 void *(*funget) (void *arg),
			 void (*funfree) (void *arg, void *data), void *arg)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;

	struct mbag_item_fundef *fundef = malloc(sizeof(struct mbag_item_fundef));
	i->data = fundef;
	if (!fundef)
		return 0;

	fundef->get = funget;
	fundef->free = funfree;
	fundef->arg = arg;
	i->type=MBAG_FUN;

	return 1;
}


#endif
