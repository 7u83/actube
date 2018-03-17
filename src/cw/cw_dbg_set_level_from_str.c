
#include "dbg.h"

int cw_dbg_set_level_from_str(const char *level)
{
	int blevel,on;
	const char *slevel;
	
	switch(*level){
		case '-':
		case '!':
			on =0;
			slevel=level+1;
			break;
		case '+':
			slevel=level+1;
			on=1;
			break;
		default:
			slevel=level;
			on=1;
	}

	blevel = cw_strlist_get_id(cw_dbg_strings, slevel);
	if (blevel==-1)
		return 0;
		
	cw_dbg_set_level(blevel,on);
	return 1;
}
