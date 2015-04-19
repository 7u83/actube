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


struct mbag_item;

struct mbag_typedef{
	void (*del)(void*);
	
};


struct mbag_item{
	uint32_t id;
	const struct mbag_typedef * type;
	union {
		void *data;
		uint8_t byte;
		uint16_t word;
		uint32_t dword;
	};
};

typedef mavl_t mbag_t;
typedef struct mbag_item mbag_item_t;



extern const struct mbag_typedef mbag_type_byte;
extern const struct mbag_typedef mbag_type_word;
extern const struct mbag_typedef mbag_type_dword;
extern const struct mbag_typedef mbag_type_mbag;
extern const struct mbag_typedef mbag_type_bstr;
extern const struct mbag_typedef mbag_type_bstr16;


#define MBAG_BYTE (&mbag_type_byte)
#define MBAG_WORD (&mbag_type_word)
#define MBAG_DWORD (&mbag_type_dword)
#define MBAG_MBAG (&mbag_type_mbag)
#define MBAG_BSTR (&mbag_type_bstr)
#define MBAG_BSTR16 (&mbag_type_bstr16)



extern mbag_t mbag_create();

extern struct mbag_item *mbag_item_create(mbag_t s, uint32_t id);



static inline int mbag_set_byte(mbag_t s, uint32_t id, uint8_t byte)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->byte = byte;
	i->type = &mbag_type_byte;
	return 1;
}

static inline int mbag_set_word(mbag_t s, uint32_t id, uint16_t word)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->word = word;
	i->type = &mbag_type_word;
	return 1;
}



static inline int mbag_set_dword(mbag_t s, uint32_t id, uint32_t dword)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->dword = dword;
	i->type = &mbag_type_dword;
	return 1;
}



static inline int mbag_set_mbag(mbag_t s, uint32_t id, mbag_t mbag)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->data = mbag;
	i->type = &mbag_type_mbag;
	return 1;
}

static inline int mbag_set_data(mbag_t s, uint32_t id, struct mbag_typedef *type, void *data)
{
	struct mbag_item *i = mbag_item_create(s, id);
	if (!i)
		return 0;
	i->data = data;
	i->type = type;
	return 1;


}

#define mbag_set_bstr(m,i,d) mbag_set_data(m,i,MBAG_BSTR,d)
#define mbag_set_bstr16(m,i,d) mbag_set_data(m,i,MBAG_BSTR16,d)


static inline mbag_item_t *mbag_get(mbag_t s, uint32_t id)
{
	mbag_item_t i;
	i.id = id;
	return mavl_get(s, &i);
}

static inline void mbag_del(mbag_t s,uint32_t id)
{
	mbag_item_t i;
	i.id = id;
	mavl_del(s, &i);
}

static inline void * mbag_get_data(mbag_t s, uint32_t id, const struct mbag_typedef * type, void * def)
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

static inline void * mbag_get_raw(mbag_t s, uint32_t id, void * def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	return i->data;
}



static inline uint8_t mbag_get_byte(mbag_t s, uint32_t id, uint8_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_BYTE)
		return def;
	return i->byte;
}

static inline uint16_t mbag_get_word(mbag_t s, uint32_t id, uint16_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_WORD)
		return def;
	return i->word;
}



static inline uint32_t mbag_get_dword(mbag_t s, uint32_t id, uint32_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_DWORD)
		return def;
	return i->dword;
}



static inline mbag_t mbag_get_mbag(mbag_t s, uint32_t id, mbag_t def)
{
	struct mbag_item *i = mbag_get(s, id);
	if (!i)
		return def;
	if (i->type != MBAG_MBAG)
		return def;
	return i->data;
}



/** @} */


#endif
