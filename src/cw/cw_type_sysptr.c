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
#include "ktv.h"


static void del ( struct cw_KTV * data )
{
	return;
}

static struct cw_KTV *get ( struct cw_KTV * data, const uint8_t * src, int len )
{
	data->type = &cw_type_sysptr;
	data->val.ptr = ((void**)(src))[0];
	return data;
}

static int put ( const struct cw_KTV *data, uint8_t * dst )
{
	((void**)dst)[0] = data->val.ptr ;
	return sizeof(void*);
}

static int to_str ( const struct cw_KTV *data, char *dst, int max_len )
{
	return sprintf(dst,"%p",data->val.ptr);
}

static struct cw_KTV *from_str ( struct cw_KTV * data, const char *src )
{
	uint8_t * s;
	s = bstr16_create_from_str(src);
	
	if ( !s )
		return NULL;

	data->type = &cw_type_bstr16;
	data->val.ptr = s;
	return data;
	
}

static int len ( struct cw_KTV * data ){
	return sizeof(void*);
}

const struct cw_Type cw_type_sysptr = {
	"Sysptr",	/* name */
	del,		/* del */
	put,		/* put */
	get,		/* get */
	to_str,		/* to_str */
	from_str,	/* from_str */
	len		/* len */
};
