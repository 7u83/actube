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
	data->type = &cw_type_bool;
	data->val.boolean = cw_get_byte(src);
	return data;
}

static int put(const cw_KTV_t *data, uint8_t * dst)
{
	return cw_put_byte(dst, data->val.boolean);
}

static int to_str(const cw_KTV_t *data, char *dst, int max_len)
{
	/*if (max_len<3){
		return 0;
	}*/
	if(data->val.boolean)
		return sprintf(dst, "%s", "true");
	else	
		return sprintf(dst, "%s", "false");
}

static cw_KTV_t *from_str(cw_KTV_t * data, const char *src)
{
	data->type = &cw_type_bool;
	if (cw_stricmp(src,"true") == 0){
		data->val.boolean=1;
		return data;
	}
	if (cw_stricmp(src,"yes") == 0){
		data->val.boolean=1;
		return data;
	}
	if (cw_stricmp(src,"on") == 0){
		data->val.boolean=1;
		return data;
	}
	if (atoi(src)==1){
		data->val.boolean=1;
		return data;
	
	}

	data->val.boolean=0;
	return data;
}

static int len (cw_KTV_t * data)
{
	return sizeof(int);
}

static void * data( cw_KTV_t * data)
{
	return & data->val.boolean;
}

static const char * get_type_name(cw_KTV_t *data)
{
	return CW_TYPE_BOOL->name;
}

const struct cw_Type cw_type_bool = {
	"Bool",			/* name */
	NULL,			/* del */
	put,			/* put */
	get,			/* get */
	to_str,			/* to_str */
	from_str,		/* from_str */ 
	len,			/* len */
	data,			/* data */ 
	get_type_name		/* get_type_name */
};
