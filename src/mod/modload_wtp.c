/*
#include "../cw/mod.h"


#include "cipwap/mod_cipwap.h"
#include "capwap/mod_capwap.h"
#include "cisco/mod_cisco.h"

#define MODS_WTP { mod_capwap_wtp, NULL }

*/

#include "modload.h"

struct mod_wtp *(*mods_wtp[])() = MODS_WTP;

struct mod_wtp * modload_wtp(const char *name)
{
	int i;
	for (i=0; mods_wtp[i];i++){

		struct mod_wtp * m = mods_wtp[i]();
		if (strcmp(m->name,name)==0)
			return m;
	}	
	return NULL;
}
