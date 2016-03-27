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
 * @brief Functions for mods
 */

#include <string.h>
#include <errno.h>


#include "action.h"
#include "mavl.h"
#include "dbg.h"
#include "log.h"


static void (*actions_registered_cb) (struct mod_ac * capwap, struct mod_ac * bindings,
				      struct cw_actiondef * actions) = NULL;



int mod_caching = 1;

void mod_set_actions_registered_cb(void (*fun)
			      (struct mod_ac *, struct mod_ac *, struct cw_actiondef *))
{
	actions_registered_cb = fun;
}


struct cache_item {
	const char *capwap;
	const char *bindings;
	struct cw_actiondef actions;

};

static struct mavl *cache = NULL;

static int mod_null_register_actions(struct cw_actiondef *def, int mode)
{
	return 0;
}

/**
 * mod_null is a dummy mod 
 */
struct mod_ac mod_null = {
	.name = "none",
	.register_actions = mod_null_register_actions
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


struct cw_actiondef *mod_cache_add(struct conn *conn, struct mod_ac *c, struct mod_ac *b)
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
		actions_registered_cb(c,b,&(i->actions));

	mavl_add(cache, i);
	return &(i->actions);
}
