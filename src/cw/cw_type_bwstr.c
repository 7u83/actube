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


/*typedef uint16_t* bwstr_type;*/


static uint16_t * bwstr_create(const uint8_t *data, uint16_t len)
{
	uint16_t * str = malloc(2+len*sizeof(uint16_t));
	if (!str)	
		return 0;
	*((uint16_t*)str)=len;
	memcpy(str+2,data,len*2);
	return str;
}



static void del ( struct cw_KTV * data )
{
	free ( data->val.ptr );
}

static struct cw_KTV *get ( struct cw_KTV * data, const uint8_t * src, int len )
{
	uint16_t * s;
	s = bwstr_create ( src, len );
	
	if ( !s )
		return NULL;

	data->type = &cw_type_bwstr;
	data->val.ptr = s;
	return data;
}

static int put ( const struct cw_KTV *data, uint8_t * dst )
{
	uint16_t len;
	len = bstr16_len(data->val.ptr);

	return lw_put_data ( dst, data->val.ptr+2,len*2 );
}

static int to_str ( const struct cw_KTV *data, char *dst, int max_len )
{
	char *d;
	uint16_t *s;
	const char * komma="";
	uint16_t i;
       
	s = (uint16_t*)data->val.ptr;
	d = dst;

	for (i=0;i<s[0];i++){
		d += sprintf( d, "%s%d",komma,s[i+1]);
		komma=",";
	}

	return d - dst;
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

static int len (cw_KTV_t * data ){
	return bstr16_len(data->val.ptr);
}

static void * data(cw_KTV_t * data)
{
	return bstr16_data(data->val.ptr);
}

static const char * get_type_name(cw_KTV_t *data)
{
	return CW_TYPE_BSTR16->name;
}

static int cast(cw_KTV_t * data)
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


const struct cw_Type cw_type_bwstr = {
	"Bwstr",	/* name */
	del,		/* del */
	put,		/* put */
	get,		/* get */
	to_str,		/* to_str */
	from_str,	/* from_str */
	len,		/* len */
	data,		/* data */
	get_type_name,	/* get_type_name */
	cast		/* cast */
};


