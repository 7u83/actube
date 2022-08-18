#include "val.h"
#include "cfg.h"
#include "dbg.h"

int cw_generic_write_l(cw_Cfg_t **cfg, const struct cw_Type*type, const char *key, uint8_t *dst, const void * param)
{
	cw_Val_t val;
	int l;
	const char *s;
	memset(&val,0,sizeof(cw_Val_t));
	val.type=type;
	val.valguard=param;
	s = cw_cfg_get_l(cfg,key,NULL);
	if (s==NULL)
		return -1;
	type->from_str(&val,s);
	l = type->put(&val,dst);
	if(type->del)
		type->del(&val);
	return l;
}

void cw_val_destroy(cw_Val_t *val)
{
	if (val->type->del)
		val->type->del(val);
	free (val);
}
