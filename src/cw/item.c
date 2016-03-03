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


#include "item.h"
#include "log.h"


const char CW_ITEM_NONE[] = "";
const char CW_ITEM_ANY[] = "*";


struct cw_itemdef *cw_item_get_by_name(const char *id, struct cw_itemdef *table)
{
	while (table->id != CW_ITEM_NONE) {
		if (!strcmp(table->id, id))
			return table;
		table++;

	}
	return NULL;
}



static int cmp(const void *x1, const void *x2)
{
	cw_itemdef_t *i1, *i2;
	i1 = (cw_itemdef_t *) x1;
	i2 = (cw_itemdef_t *) x2;

	int rc = strcmp(i1->id, i2->id);
	if (rc != 0)
		return rc;
	return strcmp(i1->sub_id, i2->sub_id);
}

const cw_itemdef_t * cw_itemdef_get(cw_itemdefheap_t t, const char *id, const char *sub_id)
{
	cw_itemdef_t idef;
	idef.id = id;
	idef.sub_id = !sub_id ? CW_ITEM_NONE : sub_id;

	return mavl_get(t, &idef);
}

cw_itemdefheap_t cw_itemdefheap_create()
{
	return mavl_create(cmp, NULL);
}

int cw_itemdefheap_register(cw_itemdefheap_t t, cw_itemdef_t * defs)
{
	int n = 0;
	while (defs->id != CW_ITEM_NONE) {
		cw_itemdef_t *a = mavl_add(t, defs);
		if (a != defs) {
			cw_log(LOG_ERR, "Duplicated item: %s", defs->id);
		} else
			n++;
		defs++;
	}
	return n;
}
