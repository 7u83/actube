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

#include <stdio.h>

#include "cw.h"
#include "cw_types.h"


static struct mdata_Elem *from_str(const char *src)
{
	struct mdata_Elem *i = mdata_elem_new(&cw_type_byte);
	if (!i)
		return NULL;

	i->data.byte = atoi(src);
	return i;
}


static int to_str(const struct mdata_Elem *e, char *dst)
{
	return sprintf(dst, "%d", e->data.byte);
}

static struct mdata_Elem *get(const uint8_t * src, int len)
{
	struct mdata_Elem *e = mdata_elem_new(&cw_type_byte);
	if (!e)
		return NULL;
	e->data.byte = cw_get_byte(src);
	return e;
}

static int put(struct mdata_Elem *e, uint8_t * dst)
{
	return cw_put_byte(dst, e->data.byte);
}

#define _I_NAME		"Byte"
#define _I_PUT		put
#define _I_GET		get
#define _I_DEL		NULL
#define _I_TO_STR	to_str
#define _I_FROM_STR	from_str

const struct mdata_Type cw_type_byte = MDATA_TYPE_INIT();
