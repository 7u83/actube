/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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

/**
 * @file 
 * @brief Functions for modules (mods) management.
 */

#include <string.h>
#include <errno.h>
#include <dlfcn.h>


#include "action.h"
#include "mavl.h"
#include "dbg.h"
#include "log.h"


static void (*actions_registered_cb) (struct cw_Mod * capwap, struct cw_Mod * bindings,
				      struct cw_actiondef * actions) = NULL;





void mod_set_actions_registered_cb(void (*fun)
				    (struct cw_Mod *, struct cw_Mod *,
				     struct cw_actiondef *))
{
	actions_registered_cb = fun;
}


struct cache_item {
	const char *capwap;
	const char *bindings;
	struct cw_actiondef actions;
	struct cw_MsgSet * msgset;
};

static struct mavl *cache = NULL;

static int mod_null_register_actions(struct cw_actiondef *def, int mode)
{
	return 0;
}

/**
 * mod_null is a dummy mod 
 */
struct cw_Mod mod_null = {
	.name = "none",
	.register_actions = mod_null_register_actions,
	
};




static int cmp(const void *p1, const void *p2)
{
	struct cache_item *c1 = (struct cache_item *) p1;
	struct cache_item *c2 = (struct cache_item *) p2;

	int r;
	r = strcmp(c1->capwap, c2->capwap);
	if (r != 0)
		return r;

	return strcmp(c1->bindings, c2->bindings);
}

struct cw_actiondef *mod_cache_get(const char *capwap, const char *bindings)
{
	return NULL;
}


struct cw_actiondef *mod_cache_add(struct conn *conn, struct cw_Mod *c, struct cw_Mod *b)
{
	if (!cache) {
		cache = mavl_create(cmp, NULL);
		if (!cache) {
			cw_log(LOG_ERR, "Can't initialize mod cache: %s",
			       strerror(errno));
			return NULL;
		}
	}

	struct cache_item s;
	s.capwap = c->name;
	s.bindings = b->name;

	struct cache_item *i = mavl_get(cache, &s);
	if (i) {
		cw_dbg(DBG_INFO, "Using cached actions for %s,%s", c->name, b->name);
		return &(i->actions);
	}


	i = malloc(sizeof(struct cache_item));
	if (!i) {
		cw_log(LOG_ERR, "Can't allocate memory for mod cache item %s",
		       strerror(errno));
		return NULL;
	}

	cw_dbg(DBG_INFO, "Loading actions for %s,%s", c->name, b->name);
	memset(i, 0, sizeof(struct cache_item));
	if (c) {
		i->capwap = c->name;
		c->register_actions(&(i->actions), MOD_MODE_CAPWAP);
	}
	if (b) {
		i->bindings = b->name;
		b->register_actions(&(i->actions), MOD_MODE_BINDINGS);
	}

	if (actions_registered_cb)
		actions_registered_cb(c, b, &(i->actions));

	mavl_add(cache, i);
	return &(i->actions);
}



/* static mavl to store modules */
static struct mavl * modlist = NULL;
static int mod_cmp(const void *e1, const void *e2){
	struct cw_Mod * (*m1fn)() = e1;
	struct cw_Mod * (*m2fn)() = e2;
	return strcmp(m1fn()->name,m2fn()->name);
}


/**
 * @brief Load a module 
 * @param name Name of the module
 * @return pointer to the module structure or NULL if the 
 * module cannot be loaded.
 */
struct cw_Mod * cw_mod_get(const char *name)
{
	
/*int i;
        for (i=0; modlist[i];i++){

                struct cw_Mod * m = modlist[i]();
                if (strcmp(m->name,name)==0)
                        return m;
        }
        return NULL;
*/
}

int cw_mod_add(struct cw_Mod * (*modfn)() ){
	if (modlist == NULL){
		modlist = mavl_create(mod_cmp,NULL);
		if (modlist==NULL){
			return 0;
		}
	}
	mavl_add(modlist,modfn);
	return 1;
}


int cw_mod_add_dynamic(const char * filename){
	void * handle;
	handle = dlopen(filename,RTLD_NOW);
	if (!handle){
		printf("Error: %s",dlerror());
	}

	printf("Load DLL %p\n",handle);

	void * ifu = dlsym(handle,"bstr_create");

	printf("IFU DLL %p\n",ifu);
}