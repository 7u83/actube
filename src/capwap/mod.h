#ifndef __MOD_H
#define __MOD_H

#include <stddef.h>

struct mod_ac
{
	const char *name;
	int (*init)();
	int (*detect_by_raw)(const char *msg, int len);
	int (*detect_by_discovery)(const char*);
};


#include "mod/cipwap/mod_cipwap.h"


#define MODS_AC { mod_cipwap_ac,NULL }



#endif
