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


/**
 *@file
 *@brief Definitions for logging and debugging.
 */

#ifndef __CW_LOG_H
#define __CW_LOG_H

#include <stdarg.h>
#include <stdint.h>
#include <syslog.h>

#ifndef LOG_ERROR
#define LOG_ERROR LOG_ERR
#endif

struct cw_LogWriter {
	const char * name;
	int colored;
	void (*open) ();
	void (*write) (int prio, const char *format, va_list args, struct cw_LogWriter *w);
	void (*close) ();
	void * priv;
};

extern struct cw_LogWriter cw_log_syslog_writer;
extern struct cw_LogWriter cw_log_console_writer;

void cw_log_init();
void cw_log(int level, const char *format, ...);

extern const char *cw_log_name;
extern struct cw_LogWriter * cw_log_writers[];


#endif
