#include <string.h>

#include <mavl.h>

#include "cw.h"
#include "cfg.h"

struct cfg_entry{
	char *key;
	char *val;
};

static int cmp(const void *k1,const void*k2){
	struct cfg_entry * e1,*e2;
	e1=(struct cfg_entry *)k1;
	e2=(struct cfg_entry *)k2;
	return strcmp(e1->key,e2->key);
}

static void del(void *ptr)
{
	struct cfg_entry * e;
	e=(struct cfg_entry *)ptr;
	free(e->key);
	free(e->val);
}


cw_Cfg_t * cfg_create()
{
	return mavl_create(cmp, del, sizeof(struct cfg_entry));
}	

int cfg_set(cw_Cfg_t * cfg,const char *key, const char *val)
{
	struct cfg_entry e;
	int replaced;

	e.key = cw_strdup(key);
	if (!e.key)
		return 0;
	e.val = cw_strdup(val);
	if (!e.val){
		free(e.key);
		return 0;
	}
	void * rc = mavl_replace(cfg,&e,&replaced);
	if (!rc){
		del(&e);
		return 0;
	}

	if (!replaced)
		return 1;
	return -1;
}

void cw_Cfg_dump(cw_Cfg_t *cfg)
{
	mavliter_t it;
	struct cfg_entry *e;
	mavliter_init(&it,cfg);
	mavliter_foreach(&it){
		
		e = mavliter_get(&it);
		printf("%s: %s\n",e->key,e->val);
		//cw_dbg(dbglevel,"%s%s :%s: %s",prefix,data->key,type->get_type_name(data), value);
	}
}
