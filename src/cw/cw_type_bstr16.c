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
#include "val.h"


static void del ( struct cw_Val * data )
{
	free ( data->val.ptr );
}

static struct cw_Val *get ( struct cw_Val * data, const uint8_t * src, int len )
{
	uint8_t * s;
	s = bstr16_create ( src, len );
	
	if ( !s )
		return NULL;

	data->type = &cw_type_bstr16;
	data->val.ptr = s;
	return data;
}

static int put ( const struct cw_Val *data, uint8_t * dst )
{
	return cw_put_bstr16 ( dst, data->val.ptr );
}

static int to_str ( const struct cw_Val *data, char *dst, int max_len )
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

static struct cw_Val *from_str ( struct cw_Val * data, const char *src )
{
	uint8_t * s;
	s = bstr16_create_from_str(src);
	
	if ( !s )
		return NULL;

	data->type = &cw_type_bstr16;
	data->val.ptr = s;
	return data;
	
}

static int len (cw_Val_t * data ){
	return bstr16_len(data->val.ptr);
}

static void * data(cw_Val_t * data)
{
	return bstr16_data(data->val.ptr);
}

static const char * get_type_name(cw_Val_t *data)
{
	return CW_TYPE_BSTR16->name;
}

static int cast(cw_Val_t * data)
{
	if (strcmp(data->type->name,CW_TYPE_BSTR16->name)==0)
		return 1;
	if (strcmp(data->type->name,CW_TYPE_STR->name)==0){
		char *src = data->val.ptr;
		CW_TYPE_BSTR16->from_str(data,src);
		free(src);
		return 1;
	}
	return 0;
}

static int bread(cw_Cfg_t *cfg, const char * key, const uint8_t *src, int len, void *param)
{
	char *d, *dst;
	dst = malloc(len*2+3);
	if (dst==NULL)
		return 0;
	d=dst;

	if ( format_is_utf8 ( src, len) ) {
		d += sprintf ( d, "%.*s", len, src );
		               
	} else {
		d += sprintf ( d, ".x" );
		d += format_hex ( d, src,len);
	}
	
	cw_cfg_set(cfg,key,dst);
	free(dst);
	return  d - dst;
}

static 	int bwrite(cw_Cfg_t *cfg, const char *key, const uint8_t *dst, void * param)
{
	return 0;
}


const struct cw_Type cw_type_bstr16 = {
	"Bstr16",	/* name */
	del,		/* del */
	put,		/* put */
	get,		/* get */
	to_str,		/* to_str */
	from_str,	/* from_str */
	len,		/* len */
	data,		/* data */
	get_type_name,	/* get_type_name */
	cast,		/* cast */
	bread,
	bwrite

};


