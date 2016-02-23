#ifndef __MOD_H
#define __MOD_H

#include <stddef.h>
#include <stdint.h>

#include "sock.h"
#include "conn.h"

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
	int (*detect)(struct conn *conn,const uint8_t *rawmsg, int rawlen,struct sockaddr *from);

	/** used for private data */
	void *data;
};


#include "mod/cipwap/mod_cipwap.h"
#include "mod/capwap/mod_capwap.h"


#define MODS_AC { mod_capwap_ac,mod_cipwap_ac,NULL }


extern struct mod_ac * cw_get_mod_ac(const char *name);

#endif
