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

#ifndef CW_LOG_DEFAULT_LOG
#define CW_LOG_DEFAULT_LOG cw_log_file
#endif
#ifndef CW_LOG_DEFAULT_VLOG
#define CW_LOG_DEFAULT_VLOG cw_log_vfile
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



/****/

/*extern void (*cw_log_cb) (int level, const char *fromat, ...);
extern void (*cw_log_vcb) (int level, const char *fromat, va_list args);
*/
/* Syslog functins */



/* Log to File functions */

/*
void cw_log_vfile(int level,const char * format, va_list args);
void cw_log_file(int level,const char *format, ...);
*/




/*

//extern void (*cw_log_debug_cb) (int type, const char *format, ...);

//extern void (*cw_log_debug_cbs[]) (const char *fromat, ...);
*/


/*
extern int cw_log_debug_dump_(int level, const uint8_t * data, int len,
			      const char *format, ...);


extern void cw_dbg_msgelem_(int msg, int msgelem, const uint8_t * msgbuf, int len);


struct conn;
extern void cw_dbg_missing_mand_elems_(struct conn *conn, int msgtyoe, int *mand);

extern void lw_dbg_elem(int msg_id, int elem_id, const uint8_t * elem_data, int elem_len);

extern int cw_log_debug_level;
extern int cw_log_str2dbglevel(const char *str);


extern const char *cw_log_name;

struct cw_dbg_cfgstrs {
	const char *name;
	int level;
};

extern struct cw_dbg_cfgstrs cw_dbg_cfgstrs[];


*/

#endif
