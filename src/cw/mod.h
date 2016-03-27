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

#ifndef __MOD_H
#define __MOD_H

#include <stddef.h>
#include <stdint.h>

#include "sock.h"
#include "conn.h"
#include "action.h"

struct cw_actiondef;

#define MOD_MAXMODS 8

enum {
	MOD_MODE_CAPWAP,
	MOD_MODE_BINDINGS
};

struct mod_ac {
	/** Name of the mod */
	const char *name;
	/** Initializion method */
	int (*init) ();

	/** init_config */
	int (*init_config) (mbag_t config);

	/** Detect capwap 
	This function is called after receiving and disassembling a complete 
	CAPWAP message. Either on Discovery Request or Join Request
	**/
	int (*detect) (struct conn * conn, const uint8_t * rawmsg, int rawlen,
		       int elems_len, struct sockaddr * from, int mode);

	/** used for private data */
	void *data;

	/** Register actions */
	int (*register_actions) (struct cw_actiondef * def,int mode);
};


/**
 * We define here struct mod_wtp in case we need 
 * different methods for WTP and AC later
 */
#define mod_wtp mod_ac

extern struct mod_ac mod_null;


#define MOD_NULL (&mod_null)

struct cw_actiondef * mod_cache_add(struct conn *conn,struct mod_ac *c, struct mod_ac *b);


#define mod_init_config(mod,cfg) (mod->init_config ? mod->init_config(cfg):1)

void mod_set_actions_registered_cb(void (*fun) (struct mod_ac *, struct mod_ac *, struct cw_actiondef *));

extern int mod_caching;

#define mod_set_caching(var) (mod_caching=var)
#define mod_get_caching() (mod_caching)


#endif
