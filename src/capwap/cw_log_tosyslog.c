
#include "cw_log.h"


void cw_vlog_(int level,const char * format, va_list args)
{
        openlog (cw_log_name, LOG_PERROR | LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DEBUG);
        vsyslog(level,format,args);
}

void cw_log_tosyslog(int level,const char *format, ...) 
{
	va_list args;
        va_start(args, format);
	cw_vlog_(level,format,args);
	va_end(args);
        closelog();
}


