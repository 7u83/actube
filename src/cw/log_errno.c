#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "log.h"

void cw_log_errno(char * format, ...)
{
	char str[2048];
	int err=errno;

	va_list args;
	va_start(args,format);
	vsprintf(str,format,args);
	va_end(args);
	
	cw_log(LOG_ERR,"%s: %s",str,strerror(err));
	
}
