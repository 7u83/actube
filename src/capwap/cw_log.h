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
#ifndef __CW_LOG_H
#define __CW_LOG_H

#include <stdarg.h>
#include <stdint.h>
#include <syslog.h>


#define DBG_CW_MSG			0x00000001
#define DBG_CW_MSGELEM			0x00000002
#define DBG_CW_MSGELEM_DMP		0x00000004
#define DBG_CW_INFO			0x00000008
#define DBG_CW_RFC			0x00000010
#define DBG_CW_PKT			0x00000020
#define DBG_CW_PKT_DMP			0x00000040

#define DBG_DTLS			0x10000000

#define DBG_ALL				0xffffffff


#define DBG_DETAIL_LINE_NUMBERS		0x00000001	
#define DBG_DETAIL_ASC_DMP		0x00000002


#define DBG_DETAIL_ALL			0xffffffff


#ifndef CW_LOG_DUMP_ROW_LEN
	#define CW_LOG_DUMP_ROW_LEN 32
#endif

#ifndef CW_LOG_DUMP_ROW_TAB_LEN
	#define CW_LOG_DUMP_ROW_TAB_LEN 8
#endif

#ifndef CW_LOG_DEFAULT_LOG
	#define CW_LOG_DEFAULT_LOG cw_log_tosyslog
#endif


extern void cw_log_dbg_(int type,const char * file, int line, const char * fromat, ...);
extern void cw_log_dbg_dmp_(int type,const char * file, int line, const uint8_t * data, int len,const char * format, ...);
extern int cw_dbg_opt_detail;
extern int cw_dbg_opt_type;



#ifdef WITH_CW_LOG
	#define cw_log(level,...) cw_log_cb(level,__VA_ARGS__)
#else
	#define cw_log(...)
#endif

#ifdef WITH_CW_LOG_DEBUG
	#define cw_log_debug0(...) cw_log_debug_cbs[0](__VA_ARGS__)
	#define cw_log_debug1(...) cw_log_debug_cbs[1](__VA_ARGS__)
	#define cw_log_debug2(...) cw_log_debug_cbs[2](__VA_ARGS__)
	#define cw_log_debug_dump(level,str,len,...) cw_log_debug_dump_(level,str,len,__VA_ARGS__)
	#define cw_log_debug0_dump(str,len,...) cw_log_debug_dump_(0,str,len,__VA_ARGS__)
	#define cw_log_debug1_dump(str,len,...) cw_log_debug_dump_(1,str,len,__VA_ARGS__)
	#define cw_log_debug2_dump(str,len,...) cw_log_debug_dump_(2,str,len,__VA_ARGS__)
	#define cw_log_debug(level,...) cw_log_debug_cbs[level](__VA_ARGS__)


	#define cw_log_dbg(type,...) cw_log_dbg_(type,__FILE__,__LINE__,__VA_ARGS__)
	#define cw_dbg(type,...) cw_log_dbg_(type,__FILE__,__LINE__,__VA_ARGS__)
	#define cw_log_dbg_dmp(type,str,len,...) cw_log_dbg_dmp_(type,__FILE__,__LINE__,str,len,__VA_ARGS__)
	#define cw_dbg_dmp(type,str,len,...) cw_log_dbg_dmp_(type,__FILE__,__LINE__,str,len,__VA_ARGS__)

#else
	#define cw_log_dbg(...)
	#define cw_dbg(...)


	#define cw_log_debug0(...) 
	#define cw_log_debug1(...) 
	#define cw_log_debug2(...) 
	#define cw_log_debug(...)
	#define cw_log_debug_dump(level,str,len) 
	#define cw_log_debug0_dump(level,str,len)
	#define cw_log_debug1_dump(level,str,len) 
	#define cw_log_debug2_dump(level,str,len) 
#endif

extern void (*cw_log_cb)(int level,const char * fromat, ...);
extern void (*cw_log_debug_cb)(int type, const char *format, ...);

extern void (*cw_log_debug_cbs[])(const char * fromat, ...);
extern int cw_log_debug_dump_(int level,const uint8_t * data, int len, const char * format, ...);
extern void cw_vlog_(int level,const char * format, va_list args);
extern void cw_log_tosyslog(int level,const char *format, ...);
extern void cw_log_tofile(int level,const char *format, ...);


extern int cw_log_debug_level;


extern const char * cw_log_name;

#endif 
