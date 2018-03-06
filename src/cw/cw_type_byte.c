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

static mavldata_t *get(mavldata_t * data, const uint8_t * src, int len)
{
	data->kv.priv = &cw_type_byte;
	data->kv.val.byte = cw_get_byte(src);
	return data;
}

static int put(mavldata_t *data, uint8_t * dst)
{
	return cw_put_byte(dst, data->kv.val.byte);
}

static int to_str(const mavldata_t *data, char *dst, int max_len)
{
	return sprintf(dst, "%d", data->kv.val.byte);
}

static mavldata_t *from_str(mavldata_t * data, const char *src)
{
	data->kv.val.byte = atoi(src);
	return data;
}


const struct cw_Type cw_type_byte = {
	"Byte",		/* name */
	NULL,		/* del */
	put,		/* put */
	get,		/* get */
	to_str,		/* to_str */
	from_str	/* from_str */ 
};

