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


#ifndef __CFGSTORE_H
#define __CFGSTORE_H


#include <stdint.h>

#include "avltree.h"
#include "bstr.h"

enum cw_cfgtem_types{
	CW_ITEMTYPE_NONE=0,
	CW_ITEMTYPE_BYTE,
	CW_ITEMTYPE_WORD,
	CW_ITEMTYPE_DWORD,
	CW_ITEMTYPE_DATA,
	CW_ITEMTYPE_CONST_DATA,
	CW_ITEMTYPE_STR,
	CW_ITEMTYPE_BSTR,
	CW_ITEMTYPE_VERSION,
	CW_ITEMTYPE_AVLTREE,
	CW_ITEMTYPE_FUN,

};

struct cw_item {
	uint32_t id;
	uint8_t type;
	union {
		void *data;
		uint8_t byte;
		uint16_t word;
		uint32_t dword;
	};
};

typedef struct avltree * cw_itemstore_t;


static inline struct cw_item * cw_itemstore_get(cw_itemstore_t s, uint32_t id)
{
	struct cw_item i;
	i.id=id;
	return avltree_get(s,&i);
}


extern cw_itemstore_t cw_itemstore_create();
extern int cw_itemstore_set_strn(cw_itemstore_t s,uint32_t id,const char *str,int n);
extern int cw_itemstore_set_str(cw_itemstore_t s,uint32_t id,const char *str);
extern int cw_itemstore_set_ptr(cw_itemstore_t s, uint32_t id, void*ptr);
extern int cw_itemstore_set_bstrn(cw_itemstore_t s, uint32_t id, uint8_t *data,int len);
extern int cw_itemstore_set_dword(cw_itemstore_t s,uint32_t id,uint32_t dword);
extern int cw_itemstore_set_byte(cw_itemstore_t s,uint32_t id,uint8_t byte);
extern int cw_itemstore_set_version(cw_itemstore_t s, uint32_t id, uint32_t vendor_id, uint8_t * versionstr, int len);

extern void *cw_item_get_data_ptr(struct cw_item *item);
extern void cw_item_release_data_ptr(struct cw_item *item, void *data);


int cw_itemstore_set_fun(cw_itemstore_t s, uint32_t id,
			 void *(*funget) (void *arg),
			 void (*funfree) (void *arg, void *data), void *arg);


#endif
