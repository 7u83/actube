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

#include "strlist.h"
#include "log.h"

/*
void (*cw_log_cb)(int level,const char * fromat, ...) = CW_LOG_DEFAULT_LOG;
void (*cw_log_vcb)(int level,const char * fromat, va_list args) = CW_LOG_DEFAULT_VLOG;
*/

const char * cw_log_name = "actube";


static struct cw_StrListElem prefix[] = {
	{LOG_DEBUG, "DBG"},
	{LOG_INFO, "INF" },
	{LOG_NOTICE, "NOTICE"},
	{LOG_WARNING, "WARNING"},
	{LOG_ERR,"ERROR"},
	{CW_STR_STOP, NULL}
};

static struct cw_StrListElem prefix_color[] = {
	{LOG_DEBUG, ""},
	{LOG_INFO, "" },
	{LOG_NOTICE, ""},
	{LOG_WARNING, ""},
	{LOG_ERR,"\033[1;31m"},
	{CW_STR_STOP, NULL}
};

static struct cw_StrListElem text_color[] = {
	{LOG_DEBUG, ""},
	{LOG_INFO, "" },
	{LOG_NOTICE, ""},
	{LOG_WARNING, ""},
	{LOG_ERR,"\033[22m"},
	{CW_STR_STOP, NULL}
};

static struct cw_StrListElem end_color[] = {
	{LOG_DEBUG, ""},
	{LOG_INFO, "" },
	{LOG_NOTICE, ""},
	{LOG_WARNING, ""},
	{LOG_ERR,"\033[22;39m"},
	{CW_STR_STOP, NULL}
};

static struct cw_LogWriter * writers[] = {
	&cw_log_syslog_writer,
	&cw_log_console_writer,
	NULL
};


void cw_log(int prio, const char *format, ...)
{
	va_list args;
	char fbuf[1024];
	int i;
	
	

	for (i=0; writers[i]; i++){
	
		if (writers[i]->colored){
			sprintf(fbuf, "%s%s%s: %s%s",
				cw_strlist_get_str(prefix_color,prio),
				cw_strlist_get_str(prefix,prio),
				cw_strlist_get_str(text_color,prio),
				format,
				cw_strlist_get_str(end_color,prio)
			);
			
		}
		else{
			sprintf(fbuf, "%s: %s",
				cw_strlist_get_str(prefix,prio),
				format
			);
			
		}
		va_start(args, format);		
		writers[i]->write(prio,fbuf,args,writers[i]);
		va_end(args);
	}

}


void cw_log_init(){
	int i;
	for (i=0; writers[i]; i++){
		writers[i]->open();
	}
}
