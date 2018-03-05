
#include <stdio.h>

#include "log.h"


static void open(){
}

static void close(){

}
static void write(int level,const char * format, va_list args, struct cw_LogWriter * w)
{
        vprintf(format,args);
	printf("\n");
}

struct cw_LogWriter cw_log_console_writer = {
	"syslog",	/* name */
	1,		/* colored */
	open,		/* open */
	write,		/* write */
	close,		/* close */
	NULL		/* priv */
}; 
