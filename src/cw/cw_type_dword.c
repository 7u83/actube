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

static struct cw_KTV *get(struct cw_KTV * data, const uint8_t * src, int len)
{
	data->type = &cw_type_dword;
	data->val.dword = cw_get_dword(src);
	return data;
}

static int put(const struct cw_KTV *data, uint8_t * dst)
{
	return cw_put_dword(dst, data->val.dword);
}

static int to_str(const struct cw_KTV *data, char *dst, int max_len)
{
	return sprintf(dst, "%d", data->val.dword);
}

static struct cw_KTV *from_str(struct cw_KTV * data, const char *src)
{
	data->type = &cw_type_dword;
	data->val.dword = atoi(src);
	return data;
}

static const char * get_type_name(cw_KTV_t *data)
{
	return CW_TYPE_DWORD->name;
}



static int cast(cw_KTV_t * data)
{
	if (strcmp(data->type->name,CW_TYPE_BYTE->name)==0)
		return 1;
	if (strcmp(data->type->name,CW_TYPE_STR->name)==0){
		char *src = data->val.ptr;
		CW_TYPE_DWORD->from_str(data,src);
		free(src);
		return 1;
	}
	return 0;
}


const struct cw_Type cw_type_dword = {
	"Dword",		/* name */
	NULL,			/* del */
	put,			/* put */
	get,			/* get */
	to_str,			/* to_str */
	from_str,		/* from_str */ 
	NULL,			/* len */
	NULL,			/* data */
	get_type_name,		/* get_type_name */
	cast
};

