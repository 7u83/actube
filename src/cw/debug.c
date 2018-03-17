#include <stdarg.h>
#include "debug.h"

#include "dbg.h"
#include "log.h"



void cw_debug(const char * file, int line, struct dbg_Context *ctx, int level,  
		const char * format, ...){
	va_list args;
	if (!(cw_dbg_is_level(level)))
		return;

/*	char fbuf[1024];

	sprintf(fbuf, "DBG:%s%s %s%s%s",
		get_dbg_color_on(level),
		get_dbg_prefix(level),
		get_dbg_color_ontext(level), format, get_dbg_color_off(level)
	    );

*/

	va_start(args, format);
	cw_log_console_writer.write(LOG_DEBUG,format,args,&cw_log_console_writer);
	va_end(args);
}

