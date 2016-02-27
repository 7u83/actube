#ifndef __MOD_H
#define __MOD_H

#include <stddef.h>
#include <stdint.h>

#include "capwap/sock.h"
#include "capwap/conn.h"
#include "action.h"

struct cw_actiondef;

struct mod_ac
{
	/** Name of the mod */
	const char *name;
	/** Initializion method */
	int (*init)();
	/** Detect capwap 
	This function ifter receiving and disassembling a complete 
	CAPWAP message. Either on Discovery Request or Join Request
	**/
	int (*detect)(struct conn *conn,const uint8_t *rawmsg, int rawlen,int elems_len, struct sockaddr *from);

	/** used for private data */
	void *data;

	int (*register_actions)(struct cw_actiondef *def);

};


//#include "cipwap/mod_cipwap.h"
//#include "capwap/mod_capwap.h"


//#define MODS_AC { mod_capwap_ac,mod_cipwap_ac,NULL }


//extern struct mod_ac * cw_get_mod_ac(const char *name);

#endif
