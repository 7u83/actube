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

#include "format.h"
#include "cw.h"
#include "kvt.h"


static void del ( struct cw_KVT * data )
{
	free ( data->val.ptr );
}

static struct cw_KVT *get ( struct cw_KVT * data, const uint8_t * src, int len )
{
	uint8_t * s;
	s = bstr16_create ( src, len );
	
	if ( !s )
		return NULL;
		
	data->type = &cw_type_bstr16;
	data->val.ptr = s;
	return data;
}

static int put ( const struct cw_KVT *data, uint8_t * dst )
{
	return cw_put_bstr16 ( dst, data->val.ptr );
}

static int to_str ( const struct cw_KVT *data, char *dst, int max_len )
{
	char *d;
	d = dst;
	
	if ( format_is_utf8 ( bstr16_data ( data->val.ptr ), bstr16_len ( data->val.ptr ) ) ) {
		d += sprintf ( d, "%.*s", bstr16_len ( data->val.ptr ),
		               bstr16_data ( data->val.ptr ) );
		               
	} else {
		d += sprintf ( d, ".x" );
		d += format_hex ( d, bstr16_data ( data->val.ptr ), bstr16_len ( data->val.ptr ) );
	}
	
	return d - dst;
}

static struct cw_KVT *from_str ( struct cw_KVT * data, const char *src )
{
	return NULL;
}


const struct cw_Type cw_type_bstr16 = {
	"Bstr16",	/* name */
	del,		/* del */
	put,		/* put */
	get,		/* get */
	to_str,		/* to_str */
	from_str	/* from_str */
};
