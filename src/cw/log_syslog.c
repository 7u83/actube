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

#include <stdarg.h>
#include <stdio.h>

#include "log.h"


static void open(){
/*	openlog (cw_log_name, LOG_PERROR | LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER); */
	openlog (cw_log_name, LOG_NDELAY, LOG_USER); 
}

static void close(){
	
}
static void write(int level,const char * format, va_list args, struct cw_LogWriter * w)
{
        vsyslog(level,format,args);
}

struct cw_LogWriter cw_log_syslog_writer = {
	"syslog",	/* name */
	0,		/* colored */
	open,		/* open */
	write,		/* write */
	close,		/* close */
	NULL		/* priv */
}; 
