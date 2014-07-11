/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "cw_log.h"



static void cw_log_debug0_(const char *format, ...) 
{
	if (cw_log_debug_level<0)
		return;
	va_list args;
        va_start(args, format);
	cw_vlog_(LOG_DEBUG,format,args);
	va_end(args);
        closelog();
}

static void cw_log_debug1_(const char *format, ...) 
{
	if (cw_log_debug_level<1)
		return;

	va_list args;
        va_start(args, format);
	cw_vlog_(LOG_DEBUG,format,args);
	va_end(args);
        closelog();
}

static void cw_log_debug2_(const char *format, ...) 
{
	if (cw_log_debug_level<2)
		return;
	va_list args;
        va_start(args, format);
	cw_vlog_(LOG_DEBUG,format,args);
	va_end(args);
        closelog();
}

int cw_log_debug_dump_(int level,const uint8_t * data, int len,const char * format, ...)
{
	int maxtlen=2048;
	int i;
	int rowlen = CW_LOG_DUMP_ROW_LEN;
	int rows = len/rowlen;
	int tlen=0;

	char * dst = malloc(len*3+(rows*2) + 8+maxtlen);
	if ( !dst )
		return 0;

	if (format != NULL){
		va_list args;
		va_start(args,format);
		tlen = vsnprintf(dst,maxtlen,format,args);
		va_end(args);
	}



	if (len%CW_LOG_DUMP_ROW_LEN)
		rows++;

	

	char * pdst=dst+tlen;
	sprintf(pdst,"\n\t");
	pdst+=2;

	for (i=0; i<len;i++){
		sprintf(pdst,"%02X ",data[i]&0xff);
		pdst+=3;
		if ((i+1)%rowlen==0){
			sprintf(pdst,"\n\t");
			pdst+=2;
		}

	}

	cw_log_debug_cbs[level]("%s",dst);

	free (dst);
	return 1;

}

int cw_log_debug_level=0;

void (*cw_log_debug_cbs[])(const char * fromat, ...) = {
	cw_log_debug0_,
	cw_log_debug1_,
	cw_log_debug2_
};


