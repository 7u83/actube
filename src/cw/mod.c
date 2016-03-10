#include <string.h>

#include "action.h"
#include "mavl.h"
#include "dbg.h"

struct cache_item
{
	const char *capwap;
	const char *bindings;
	struct cw_actiondef actions;

};

static struct mavl * cache = NULL;

static int mod_null_register_actions(struct cw_actiondef *def,int mode)
{
	return 0;
}
struct mod_ac mod_null={
	.name="none",
	.register_actions=mod_null_register_actions
};




int cmp(const void *p1, const void *p2)
{
	struct cache_item * c1 = (struct cache_item *)p1;
	struct cache_item * c2 = (struct cache_item *)p2;

	int r;
	r = strcmp(c1->capwap,c2->capwap);
	if (r!=0)
		return r;

	return strcmp(c1->bindings,c2->bindings);
	
}

struct cw_actiondef * mod_cache_get(const char *capwap, const char *bindings)
{

	return NULL;
}

struct cw_actiondef * mod_cache_add(struct mod_ac *c, struct mod_ac *b)
{
	if (!cache){
		cache = mavl_create(cmp,NULL);
		if (!cache)
			return NULL;
	}

	struct cache_item s;
	s.capwap=c->name;
	s.bindings=b->name;

	struct cache_item * i  = mavl_get(cache,&s);
	if (i){
		cw_dbg(DBG_INFO,"Using cached actions for %s,%s",c->name,b->name);
		return &(i->actions);
	}


	i = malloc(sizeof(struct cache_item));
	if (!i)
		return NULL;

	cw_dbg(DBG_INFO,"Loading actions for %s,%s",c->name,b->name);
	memset (i,0,sizeof(struct cache_item));
	if (c){
		i->capwap=c->name;
		c->register_actions(&(i->actions),MOD_MODE_CAPWAP);
	}
	if (b){
		i->bindings=b->name;
		b->register_actions(&(i->actions),MOD_MODE_BINDINGS);
	}

	mavl_add(cache,i);

	return &(i->actions);

}
