
#include <string.h>


#include "mod.h"

#include <stdio.h>


struct cw_Mod *(*mods_ac[])() = MODS_AC;


struct cw_Mod * cw_get_mod_ac(const char *name)
{
	int i;
	for (i=0; mods_ac[i];i++){

		struct cw_Mod * m = mods_ac[i]();
		if (strcmp(m->name,name)==0)
			return m;
	}	
	return NULL;
}
