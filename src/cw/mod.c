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


#include "mavl.h"
#include "dbg.h"
#include "log.h"
#include "file.h"
#include "cw.h"
#include "msgset.h"


/*
static int mod_null_register_actions(struct cw_actiondef *def, int mode)
{
	return 0;
}
*/

/**
 * mod_null is a dummy mod 
 */
struct cw_Mod mod_null = {
	"none",			/* name */
	NULL,			/* init */
	NULL,			/* init_config */
	NULL,			/* detect */
	NULL			/* data */
};





struct cw_MsgSet *cw_mod_get_msg_set(struct cw_Conn *conn,
				     struct cw_Mod *capwap_mod,
				     struct cw_Mod *bindings_mod)
{
	struct cw_MsgSet *set;

	
	set = cw_msgset_create();
	if (!set) {
		cw_log(LOG_ERR, "Can't allocate memory for mod  %s",
		       strerror(errno));
		return NULL;
	}

	cw_dbg(DBG_INFO, "Loading message set for %s,%s", capwap_mod->name,
	       bindings_mod->name);

	
	if (capwap_mod != MOD_NULL) {
		capwap_mod->register_messages(set, CW_MOD_MODE_CAPWAP);
	}
	if (bindings_mod != MOD_NULL) {
		bindings_mod->register_messages(set, CW_MOD_MODE_BINDINGS);
	}
	return set;
}



/* static mavl to store modules */
static struct mavl *mods_loaded = NULL;
static int mod_cmp_mavl(const void *e1, const void *e2)
{
	const struct cw_Mod *m1 = *((const struct cw_Mod **) e1);
	const struct cw_Mod *m2 = *((const struct cw_Mod **) e2);
	return strcmp(m1->name, m2->name);
}

static int mod_cmp_mlist(const void *e1, const void *e2)
{
	const struct cw_Mod *m1 = e1;
	const struct cw_Mod *m2 = e2;
	return strcmp(m1->name, m2->name);
}



static const char *mod_path = "";

/**
 * @brief Set module path, where to search for modules
 * @param path Path to search
 */
void cw_mod_set_path(const char *path)
{
	mod_path = path;
}

/**
 * @brief Load a module 
 * @param mod_name Name of the module
 * @return a pointer to the module interface
 */
struct cw_Mod *cw_mod_load(const char *mod_name, cw_Cfg_t * global_cfg, int role)
{
	struct cw_Mod search;
	struct cw_Mod *mod;
	char mod_filename[CW_MOD_MAX_MOD_NAME_LEN + 5];
	char *filename;
	void *handle;

	/* if modlist is not initialized, initialize ... */
	if (mods_loaded == NULL) {
		mods_loaded = mavl_create(mod_cmp_mavl, NULL, sizeof(struct cw_Mod*));
		if (mods_loaded == NULL) {
			cw_log(LOG_ERROR, "Can't init modlist, no memory");
			return NULL;
		}
	}

	/* Search for the module in mods_loaded, to see if it is
	 * already loaded or was statically linked */

	cw_dbg(DBG_MOD, "MOD: Load module '%s'", mod_name);

	memset(&search, 0, sizeof(search));
	search.name = mod_name;

	mod = mavl_get_ptr(mods_loaded, &search);
	if (mod) {
		cw_dbg(DBG_MOD, "MOD: Module already loaded '%s'", mod_name);
		return mod;
	}

	if (strlen(mod_name) > CW_MOD_MAX_MOD_NAME_LEN) {
		cw_log(LOG_ERROR, "Mod name too long: %s (max allowed = %d)",
		       mod_name, CW_MOD_MAX_MOD_NAME_LEN);
		return NULL;
	}


	sprintf(mod_filename, "mod_%s", mod_name);

	/* we have to load the module dynamically */

	filename = cw_filename(mod_path, mod_filename, ".so");
	if (filename == NULL)
		return NULL;

	cw_dbg(DBG_MOD, "MOD: loading module from file: %s", filename);

	/* Open the DLL */
	handle = dlopen(filename, RTLD_NOW);

	if (!handle) {
		cw_log(LOG_ERROR, "Failed to load module: %s", dlerror());
		goto errX;
	}

	mod = dlsym(handle, mod_filename);
	if (mod == NULL) {
		cw_log(LOG_ERROR, "Failed to load module: %s", dlerror());
		goto errX;
	}

	mod->dll_handle = handle;

	if (!mavl_insert_ptr(mods_loaded, mod)) {
		dlclose(handle);
		cw_log(LOG_ERR, "Can' add module %s", mod_name);
		goto errX;
	}

	cw_dbg(DBG_MOD, "MOD: %s sucessfull loaded, calling init now.", filename);
	mod->init(mod, global_cfg, role);
	
      errX:
	free(filename);
	return mod;
}


static struct mlist *mods_list = NULL;

struct cw_Mod *cw_mod_add_to_list(struct cw_Mod *mod)
{
	mlistelem_t *elem;
	if (!mods_list) {
		mods_list = mlist_create(mod_cmp_mlist, NULL, sizeof(struct cw_Mod *));
		if (!mods_list) {
			cw_log(LOG_ERROR, "Can't init mods_list");
			return 0;
		}
	}

	elem = mlist_append(mods_list, &mod);
	if (elem == NULL)
		return NULL;
	return mlistelem_dataptr(elem);
}

struct cw_Mod *cw_mod_detect(struct cw_Conn *conn,
			     uint8_t * rawmsg, int len,
			     int elems_len, struct sockaddr *from, int mode)
{

	struct mlistelem *e;

	if (mods_list == NULL)
		return MOD_NULL;


	mlist_foreach(e, mods_list) {

		struct cw_Mod *mod = *(struct cw_Mod **) (mlistelem_dataptr(e));	/* = e->data; */
		cw_dbg(DBG_MOD, "Checking mod: %s", mod->name);

		/* if there is no detect method, skip */
		if (!mod->detect)
			continue;

		if (mod->detect(conn, rawmsg, len, elems_len, from, mode)) {
			return mod;
		}
	}
	return MOD_NULL;
}
