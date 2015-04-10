
#include <stdio.h>

#include "log.h"


int cw_log_file_flags = LOG_PERROR;
char * cw_log_filename = NULL;


void cw_vlog_tofile_(int level,const char * format, va_list args)
{
	switch(level){
		case LOG_DEBUG:
			printf("DEBUG:");
			break;
		case LOG_INFO:
			printf("INFO:");
			break;
	}
	vprintf(format,args);

}

void cw_log_tofile(int level,const char *format, ...) 
{
	va_list args;
        va_start(args, format);

//	cw_vlog_(level,format,args);
	cw_vlog_tofile_(level,format,args);
	va_end(args);
        closelog();
}


