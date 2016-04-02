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


#ifndef __CW_ITEM_H
#define __CW_ITEM_H

#include "mbag.h"

extern const char CW_ITEM_NONE[];
extern const char CW_ITEM_ANY[];


/* Definitions for CAPWAP Items */

struct cw_itemdef {
	/** ID */
	const char *id;
	/** Sub-ID */
	const char *sub_id;
	/** Type */
	mbagtype_t type;
	/** 
	 * A category this item belongs to.
	 */
	const char *cat;

};
typedef struct cw_itemdef cw_itemdef_t;


typedef mavl_t cw_itemdefheap_t;
extern cw_itemdefheap_t cw_itemdefheap_create();
extern int cw_itemdefheap_register(cw_itemdefheap_t t, cw_itemdef_t * defs);
extern const cw_itemdef_t * cw_itemdef_get(cw_itemdefheap_t t, const char *id,
				   const char *sub_id);




struct cw_itemdef *cw_item_get_by_name(const char *name, struct cw_itemdef *table);

typedef const char *cw_item_id_t;
//
#endif
