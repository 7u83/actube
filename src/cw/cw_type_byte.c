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

static const char * get_guardstr(int val, const cw_KTVValRange_t * valrange)
{
	while(valrange->name!=NULL){
		if(val>=valrange->min && val<=valrange->max)
			return valrange->name;
		valrange++;
	}
	return NULL;
}


static int to_str(const cw_KTV_t *data, char *dst, int max_len)
{
	if (data->valguard!=NULL){
		const char * name;
		name = get_guardstr(data->val.byte,data->valguard);
		if (name != NULL){
			return sprintf(dst,"%s",name);
		}
	}
	
		/*if (max_len<3){
			return 0;
		}*/
	return sprintf(dst, "%d", data->val.byte);

}

static cw_KTV_t *from_str(cw_KTV_t * data, const char *src)
{
	data->type = &cw_type_byte;
	data->val.byte = atoi(src);
	return data;
}

static int len (cw_KTV_t * data)
{
	return sizeof(data->val.byte);
}

static void * data(cw_KTV_t * data)
{
	return &data->val.byte;
}

static const char * get_type_name(cw_KTV_t *data)
{
	if (data->valguard != NULL){
		return CW_TYPE_STR->name;
	}
	return CW_TYPE_BYTE->name;
}

const struct cw_Type cw_type_byte = {
	"Byte",			/* name */
	NULL,			/* del */
	put,			/* put */
	get,			/* get */
	to_str,			/* to_str */
	from_str,		/* from_str */ 
	len,			/* len */
	data,			/* data */
	get_type_name		/* get_type_name */
};
