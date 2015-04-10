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

#include <stdio.h>
#include <stdarg.h>

#include "log.h"

void (*cw_log_cb)(int level,const char * fromat, ...) = CW_LOG_DEFAULT_LOG;
void (*cw_log_vcb)(int level,const char * fromat, va_list args) = CW_LOG_DEFAULT_VLOG;


const char * cw_log_name = "cw";
static int colored=1;


static const char * get_log_prefix(int level)
{
	switch(level){
		case LOG_DEBUG:
			return "DBG";
		case LOG_INFO:
			return "INF";
		case LOG_ERR:
			return "ERROR";
		case LOG_WARNING:
			return "WARNING";

	}
	return "";
}

static const char * get_log_color_on(int level){
	if ( !colored ) 
		return "";


	switch(level){
		case LOG_DEBUG:
			return "";
		case LOG_INFO:
			return "";
		case LOG_ERR:
			return "\033[1;31m";
	}
	return "";

}

static const char * get_log_color_ontext(int level){
	if ( !colored ) 
		return "";


	switch(level){
		case LOG_DEBUG:
			return "";
		case LOG_INFO:
			return "";
		case LOG_ERR:
			return "\033[22m";
	}
	return "";

}




static const char * get_log_color_off(int level){
	if ( !colored ) 
		return "";


	switch(level){
		case LOG_DEBUG:
			return "";
		case LOG_INFO:
			return "";
		case LOG_ERR:
			return "\033[22;39m";
	}
	return "";

}





void cw_log_colored(int level, const char *format, ...)
{
	char fbuf[1024];

	sprintf(fbuf, "%s%s%s: %s%s",
		get_log_color_on(level),
		get_log_prefix(level),
		get_log_color_ontext(level),
		format,
		get_log_color_off(level)
		);
		

	va_list args;
	va_start(args, format);
	cw_log_vcb(level,fbuf,args);
	va_end(args);

}


