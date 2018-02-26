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
 * @brief module definitions
 */
  
#ifndef __MOD_H
#define __MOD_H

#include <stddef.h>
#include <stdint.h>

#include "sock.h"
#include "conn.h"
#include "action.h"

struct cw_actiondef;


enum {
	MOD_MODE_CAPWAP,
	MOD_MODE_BINDINGS
};

struct cw_Mod {
	/** Name of the module */
	const char *name;
	/** Initializion method */
	int (*init) ();

	/** init_config */
	int (*init_config) (mbag_t config);

	/** Detect capwap 
	 * This function is called after receiving and disassembling a complete 
	 * CAPWAP message. Either on Discovery Request or Join Request.
	 * @param conn current connection
	 * @return 0 if notdetected 
	**/
	int (*detect) (struct conn * conn, const uint8_t * rawmsg, int rawlen,
		       int elems_len, struct sockaddr * from, int mode);

	/** used for private data */
	void *data;

	/** Register actions */
	int (*register_actions) (struct cw_actiondef * def,int mode);
	
	struct cw_MsgSet * (*register_messages)(struct cw_MsgSet set, int mode);
	
	/** 
	 * Handle returned by dlopen, if this module was loaded 
	 * dynamically with dlopen. If this module was statically 
	 * linked, dll_handle is NULL.
	 */
	void * dll_handle;
};


extern struct cw_Mod mod_null;


#define MOD_NULL (&mod_null)

struct cw_actiondef * mod_cache_add(struct conn *conn,struct cw_Mod *c, struct cw_Mod *b);


#define mod_init_config(mod,cfg) (mod->init_config ? mod->init_config(cfg):1)

void mod_set_actions_registered_cb(void (*fun) (struct cw_Mod *, struct cw_Mod *, struct cw_actiondef *));

extern int mod_caching;

#define mod_set_caching(var) (mod_caching=var)
#define mod_get_caching() (mod_caching)

struct cw_Mod * cw_mod_add_dynamic(const char * path, const char * file);

#endif
