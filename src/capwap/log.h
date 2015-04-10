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







/** 
 * @defgroup DebugOptions Dbug Options
 * @{
 */

#define DBG_MSG				0x00000001	/* Parsed CAPWAP/LWAPP messages */

#define DBG_ELEM			0x00000002	/* Parsed CAPWAP/LWAPP message elements */
#define DBG_ELEM_DMP			0x00000004	/* Dump CAPWAP message elements */

#define DBG_CW_INFO			0x00000008
#define DBG_CW_RFC			0x00000010	/* RCF-realted CAPWAP errors */
#define DBG_CW_STRICT			0x00000010
#define DBG_CW_PKT_IN			0x00000020	/* Headers of incoming CAPWAP packets */
#define DBG_CW_PKT_OUT			0x00000040	/* Headers of outgoing CAPWAP packets */
#define DBG_CW_PKT_DMP			0x00000080	/* Dump packts */
#define DBG_CW_PKT_DTL			0x00000100
#define DBG_CW_PKT_ERR			0x00000200
#define DBG_MSG_ERR			0x00000400	/* Errors in CAPWAP messages */
#define DBG_CW_IMG_DTL			0x00000800	/* Detail about image transfer */
#define DBG_ELEM_ERR			0x00001000

/* driver specific debugs */
#define DBG_DRV				0x00010000
#define DBG_DRV_ERR			0x00020000

/* DTLS debugs */
#define DBG_DTLS			0x10000000
#define DBG_DTLS_DETAIL			0x20000000
#define DBG_DTLS_BIO			0x40000000
#define DBG_DTLS_BIO_DMP		0x80000000

#define DBG_ALL				0xffffffff
#define DBG_CW_PKT_INOUT		(DBG_CW_PKT_IN | DBG_CW_PKT_OUT)


#define DBG_DETAIL_LINE_NUMBERS		0x00000001
#define DBG_DETAIL_ASC_DMP		0x00000002


#define DBG_DETAIL_ALL			0xffffffff
#define DBG_ERR				(DBG_MSG_ERR | DBG_CW_PKT_ERR)

/**@}*/





extern void cw_log_dbg_(int type, const char *file, int line, const char *fromat, ...);
extern void cw_log_dbg_dmp_(int type, const char *file, int line, const uint8_t * data,
			    int len, const char *format, ...);
extern int cw_dbg_opt_detail;
extern int cw_dbg_opt_level;


#define cw_dbg_is_level(level) (cw_dbg_opt_level & level)



#ifdef WITH_CW_LOG
#define cw_log(level,...) cw_log_colored(level,__VA_ARGS__)
#else
#define cw_log(...)
#endif

#ifdef WITH_CW_LOG_DEBUG


#define cw_log_dbg(type,...) cw_log_dbg_(type,__FILE__,__LINE__,__VA_ARGS__)
#define cw_dbg(type,...) cw_log_dbg_(type,__FILE__,__LINE__,__VA_ARGS__)
#define cw_log_dbg_dmp(type,str,len,...) cw_log_dbg_dmp_(type,__FILE__,__LINE__,str,len,__VA_ARGS__)
#define cw_dbg_dmp(type,str,len,...) cw_log_dbg_dmp_(type,__FILE__,__LINE__,str,len,__VA_ARGS__)
#define cw_dbg_msgelem(msgtype,msgelemtype,msgbuf,msglen) cw_dbg_msgelem_(msgtype,msgelemtype,msgbuf,msglen)
#define cw_dbg_missing_mand_elems(conn, msgtyoe, mand) cw_dbg_missing_mand_elems_(conn, msgtyoe, mand)

#define lw_dbg_elem(msgtype,msgelemtype,msgbuf,msglen) lw_dbg_elem_(msgtype,msgelemtype,msgbuf,msglen)

#else
#define cw_log_dbg(...)
#define cw_dbg(...)

#define cw_dbg_missing_mand_elems(conn, msgtyoe, mand)

#endif

extern void (*cw_log_cb) (int level, const char *fromat, ...);
extern void (*cw_vlog_cb) (int level, const char *fromat, va_list args);

/* Syslog functins */
extern void cw_log_syslog(int level, const char *format, ...);
extern void cw_log_vsyslog(int level,const char * format, va_list args);

/* Log to File functions */
void cw_log_vfile(int level,const char * format, va_list args);
void cw_log_file(int level,const char *format, ...);



void cw_log_colored(int level, const char *format, ...);



//extern void (*cw_log_debug_cb) (int type, const char *format, ...);

//extern void (*cw_log_debug_cbs[]) (const char *fromat, ...);
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




#endif
