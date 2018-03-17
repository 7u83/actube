
#include "dbg.h"

/**
  * Set debug level
  * @param level debug level to set, allowed values are enumberated in #cw_dbg_levels structure.
  * @param on 1: turns the specified debug level on, 0: turns the specified debug level off.
  */

void cw_dbg_set_level (int level, int on)
{
	switch (level) {
		case DBG_ALL:
			if (on)
				cw_dbg_opt_level = 0xffffffff;
			else
				cw_dbg_opt_level = 0;
			break;
		default:
			if (on)
				cw_dbg_opt_level |= (1 << (level));
			else 
				cw_dbg_opt_level &= (0xffffffff) ^ (1 << (level));
	}
}

