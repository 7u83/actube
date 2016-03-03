
#include <string.h>


#include "mod.h"

#include <stdio.h>


struct mod_ac *(*mods_ac[])() = MODS_AC;


struct mod_ac * cw_get_mod_ac(const char *name)
{
	int i;
	for (i=0; mods_ac[i];i++){

		struct mod_ac * m = mods_ac[i]();
		if (strcmp(m->name,name)==0)
			return m;
	}	
	return NULL;
}
