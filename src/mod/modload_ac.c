#include "../capwap/mod.h"


#include "cipwap/mod_cipwap.h"
#include "capwap/mod_capwap.h"
#include "cisco/mod_cisco.h"

#define MODS_AC { mod_capwap_ac,mod_cipwap_ac, mod_cisco_ac, NULL }


struct mod_ac *(*mods_ac[])() = MODS_AC;

struct mod_ac * modload_ac(const char *name)
{
	int i;
	for (i=0; mods_ac[i];i++){

		struct mod_ac * m = mods_ac[i]();
		if (strcmp(m->name,name)==0)
			return m;
	}	
	return NULL;
}
