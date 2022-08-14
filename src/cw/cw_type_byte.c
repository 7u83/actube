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
#include "val.h"

static cw_Val_t *get(cw_Val_t * data, const uint8_t * src, int len)
{
	data->type = &cw_type_byte;
	data->val.byte = cw_get_byte(src);
	return data;
}

static int put(const cw_Val_t *data, uint8_t * dst)
{
	return cw_put_byte(dst, data->val.byte);
}

static const char * get_guardstr(int val, const cw_ValValRange_t * valrange)
{
	if (valrange==NULL)
		return NULL;

	while(valrange->name!=NULL){
		if(val>=valrange->min && val<=valrange->max)
			return valrange->name;
		valrange++;
	}
	return NULL;
}


static int to_str(const cw_Val_t *data, char *dst, int max_len)
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

static int get_guardval(const char *str, const cw_ValValRange_t * valrange)
{
	while(valrange->name!=NULL){
		if(strcmp(str,valrange->name)==0)
			return valrange->min;
		valrange++;
	}
	return -1;
}


static cw_Val_t *from_str(cw_Val_t * data, const char *src)
{
	data->type = &cw_type_byte;
	if (data->valguard != NULL){
		int rc;
		rc = get_guardval(src,data->valguard);
		if (rc != -1){
			data->val.byte = rc;
			return data;
		}
		
	}
	

	data->val.byte = atoi(src);
	return data;
}

static int len (cw_Val_t * data)
{
	return sizeof(data->val.byte);
}

static void * data(cw_Val_t * data)
{
	return &data->val.byte;
}

static const char * get_type_name(cw_Val_t *data)
{
	if (data->valguard != NULL){
		return CW_TYPE_STR->name;
	}
	return CW_TYPE_BYTE->name;
}

static int cast(cw_Val_t * data)
{
	if (strcmp(data->type->name,CW_TYPE_BYTE->name)==0)
		return 1;
	if (strcmp(data->type->name,CW_TYPE_STR->name)==0){
		char *src = data->val.ptr;
		CW_TYPE_BYTE->from_str(data,src);
		free(src);
		return 1;
	}
	return 0;
}

static int bread(cw_Cfg_t *cfg, const char * key, const uint8_t *src, int len, const void *param)
{
	uint8_t	val;
	cw_ValValRange_t * valrange = (cw_ValValRange_t *) param;
	const char *str;
	
      	val = cw_get_byte(src);
	str = get_guardstr(val, valrange);
	if (str != NULL)
		cw_cfg_set(cfg,key,str);
	else
		cw_cfg_set_int(cfg,key,val);

	return 1;
}

static 	int bwrite(cw_Cfg_t ** cfgs, const char *key, uint8_t *dst, const void * param)
{
	return cw_generic_write_l(cfgs,CW_TYPE_BYTE,key,dst,param);
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
	get_type_name,		/* get_type_name */
	cast,
	bread,
	bwrite

};
