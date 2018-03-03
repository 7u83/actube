#include <stdarg.h>
#include "debug.h"
#include "mbag.h"
#include "dbg.h"
#include "log.h"

void cw_debug(const char * file, int line, struct dbg_Context *ctx, int level,  
		const char * format, ...){
	if (!(cw_dbg_is_level(level)))
		return;

/*	char fbuf[1024];

	sprintf(fbuf, "DBG:%s%s %s%s%s",
		get_dbg_color_on(level),
		get_dbg_prefix(level),
		get_dbg_color_ontext(level), format, get_dbg_color_off(level)
	    );

*/
	va_list args;
	va_start(args, format);
	cw_log_vcb(level, format, args);
	va_end(args);
}

