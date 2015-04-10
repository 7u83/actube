
#include <stdio.h>

#include "log.h"


int cw_log_file_flags = LOG_PERROR;
char * cw_log_filename = NULL;


void cw_log_vfile(int level,const char * format, va_list args)
{
/*	switch(level){
		case LOG_DEBUG:
			printf("DBG:");
			break;
		case LOG_INFO:
			printf("INFO:");
			break;
		case LOG_ERR:
			printf("ERROR:");
			break;
	}
*/
	vprintf(format,args);
	printf("\n");

}

void cw_log_file(int level,const char *format, ...) 
{
	va_list args;
        va_start(args, format);
	cw_log_vfile(level,format,args);
	va_end(args);
        closelog();
}


