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
#include "ktv.h"

static cw_KTV_t *get(cw_KTV_t * data, const uint8_t * src, int len)
{
	data->type = &cw_type_byte;
	data->val.byte = cw_get_byte(src);
	return data;
}

static int put(const cw_KTV_t *data, uint8_t * dst)
{
	return cw_put_byte(dst, data->val.byte);
}

static int to_str(const cw_KTV_t *data, char *dst, int max_len)
{
	return sprintf(dst, "%d", data->val.byte);
}

static cw_KTV_t *from_str(cw_KTV_t * data, const char *src)
{
	data->val.byte = atoi(src);
	return data;
}


const struct cw_Type cw_type_byte = {
	"Byte",			/* name */
	NULL,			/* del */
	put,			/* put */
	get,			/* get */
	to_str,			/* to_str */
	from_str		/* from_str */ 
};
