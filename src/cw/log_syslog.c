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

#include "log.h"


void cw_log_vsyslog(int level,const char * format, va_list args)
{
        openlog (cw_log_name, LOG_PERROR | LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DEBUG);
        vsyslog(level,format,args);
}

void cw_log_syslog(int level,const char *format, ...) 
{
	va_list args;
        va_start(args, format);
	cw_log_vsyslog(level,format,args);
	va_end(args);
        closelog();
}

