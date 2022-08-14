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

#include "val.h"
#include "cw.h"


static struct cw_Val *get(struct cw_Val * data, const uint8_t * src, int len)
{
	data->type = &cw_type_word;
	data->val.word = cw_get_word(src);
	return data;
}

static int put(const cw_Val_t *data, uint8_t * dst)
{
	return cw_put_word(dst, data->val.word);
}

static int to_str(const cw_Val_t *data, char *dst, int max_len)
{
	return sprintf(dst, "%d", data->val.word);
}

static cw_Val_t *from_str(cw_Val_t * data, const char *src)
{
	data->type = &cw_type_word;	
	data->val.word = atoi(src);
	return data;
}

static const char * get_type_name(cw_Val_t *data)
{
	return CW_TYPE_WORD->name;
}
static int cast(cw_Val_t * data)
{
	if (strcmp(data->type->name,CW_TYPE_WORD->name)==0)
		return 1;
	if (strcmp(data->type->name,CW_TYPE_STR->name)==0){
		char *src = data->val.ptr;
		CW_TYPE_WORD->from_str(data,src);
		free(src);
		return 1;
	}
	return 0;
}

static int bread(cw_Cfg_t *cfg, const char * key, const uint8_t *src, int len, const void *param)
{
	uint16_t val;
	//cw_ValValRange_t * valrange = (cw_ValValRange_t *) param;
	//const char *str;
	
      	val = cw_get_word(src);
/*	str = get_guardstr(val, valrange);
	if (str != NULL)
		cw_cfg_set(cfg,key,str);
	else*/
	cw_cfg_set_int(cfg,key,val);

	return 2;
}

static 	int bwrite(cw_Cfg_t **cfgs, const char *key, uint8_t *dst, const void * param)
{
	return cw_generic_write_l(cfgs,CW_TYPE_WORD,key,dst,param);

/*	cw_Val_t val;
	int l;
	const char *s;
	memset(&val,0,sizeof(cw_Val_t));
	val.valguard=param;
	s = cw_cfg_get_l(cfg,key,NULL);
	if (s==NULL)
		return -1;
	from_str(&val,s);
	l = put(&val,dst);
	if(val.type->del)
		val.type->del(&val);
	return l;*/
}


const struct cw_Type cw_type_word = {
	"Word",			/* name */
	NULL,			/* del */
	put,			/* put */
	get,			/* get */
	to_str,			/* to_str */
	from_str,		/* from_str */ 
	NULL,
	NULL,
	get_type_name,
	cast,
	bread,
	bwrite,
	
};

