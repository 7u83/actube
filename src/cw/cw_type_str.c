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
	free ( data->val.str );
}

static struct cw_KTV *get ( struct cw_KTV * data, const uint8_t * src, int len )
{
	uint8_t * s;
	s = malloc (len+1);
	if ( !s )
		return NULL;

	memcpy(s,src,len);
	s[len]=0;

	data->type = &cw_type_str;
	data->val.str = (char*)s;
	return data;
}

static int put ( const struct cw_KTV *data, uint8_t * dst )
{
	return cw_put_str ( dst, (uint8_t*)data->val.str );
}

static int to_str ( const struct cw_KTV *data, char *dst, int max_len )
{

	
	int l;
	l = strlen(data->val.str);
	strcpy(dst,data->val.str);
	return l;
	
	if (l<max_len){
		strcpy(dst,data->val.str);
		return l;
	}
	
	memcpy(dst,data->val.str,max_len);

	dst[max_len]=0;
	return max_len;
}

static struct cw_KTV *from_str ( struct cw_KTV * data, const char *src )
{
	char * s;
	s = cw_strdup(src);
	
	if ( !s )
		return NULL;

	data->type = &cw_type_str;
	data->val.str = s;
	return data;
}

static int len ( struct cw_KTV * data ){
	return strlen (data->val.str);
}

static const char * get_type_name(cw_KTV_t *data)
{
	return CW_TYPE_STR->name;
}



const struct cw_Type cw_type_str = {
	"Str",		/* name */
	del,		/* del */
	put,		/* put */
	get,		/* get */
	to_str,		/* to_str */
	from_str,	/* from_str */
	len,		/* len */
	NULL,		/* data */
	get_type_name	/* get_type_name */
};
