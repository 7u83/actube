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
 * @file
 * @brief
 */


#ifndef __DBG_H
#define __DBG_H

#include "conn.h"
#include "cw.h"


#ifndef CW_LOG_DUMP_ROW_LEN
#define CW_LOG_DUMP_ROW_LEN 32
#endif

#ifndef CW_LOG_DUMP_ROW_TAB_LEN
#define CW_LOG_DUMP_ROW_TAB_LEN 8
#endif


/**
 *@defgroup DBG DBG
 *@{
 */

/**
 * Debug levels
 */ 
enum cw_dbg_levels{
	/** Show headers of incomming CAPWAP packets */
	DBG_PKT_IN=0,
	
	/** Show headers of outgoing CAPWAP packets */
	DBG_PKT_OUT,

	/** Incomming CAPWAP packets with errors, wich would
	    usually silently discarded */ 
	DBG_PKT_ERR,

	/** Dump content of incomming packets */
	DBG_PKT_DMP,

	/** Display incomming CAPWAP/LWAPP messages */
	DBG_MSG_IN,

	/** Display outgoing CAPWAP/LWAPP messages */
	DBG_MSG_OUT,

	/** Message errors */
	DBG_MSG_ERR,

	/** Show message elements  */
	DBG_ELEM,

	/** Show message element details  */
	DBG_ELEM_DETAIL,

	/** Error in msg elements */
	DBG_ELEM_ERR,

	/** Show subelements */
	DBG_SUBELEM,

	/** Show dump of subelements */
	DBG_SUBELEM_DMP,

	/** hex dump elements */	
	DBG_ELEM_DMP,

	/** General infos, like CAPWAP state */
	DBG_INFO,	

	/** Misc. warnings */
	DBG_WARN,

	/** RFC related */
	DBG_RFC,

	/** DTLS related messages */
	DBG_DTLS,

	/** DTLS BIOs in/out */
	DBG_DTLS_BIO,

	/** Dump DTLS BIO i/o */
	DBG_DTLS_BIO_DMP,

	/** Show DTLS Details */
	DBG_DTLS_DETAIL,

	/** Debug Mods */
	DBG_MOD,
	
	DBG_ALL, 
	
	DBG_X
};


#define DBG_LN __FILE__,__LINE__


struct dbg_Context{
	int level;
};

void cw_dbg_set_level (int level, int on);
int cw_dbg_set_level_from_str(const char *level);


void cw_dbg_elem_(struct conn * conn, int msg, int msgelem, const uint8_t * msgbuf, int len);

/*
void cw_dbg_missing_mand(int level,struct conn *conn,cw_action_in_t ** ml,int n,cw_action_in_t *a);
*/

void cw_dbg_packet(struct conn *conn, uint8_t * packet, int len);
void cw_dbg_pkt(int level,struct conn *conn, uint8_t * packet, int len,struct sockaddr *from);
void cw_dbg(int level, const char *format, ...);
void cw_dbg_dmp(int level, const uint8_t * data, int len, const char *format, ...);


#endif

/** 
 * @defgroup DbgOptions Debug Options
 * @{
 */


#define DBG_DISP_LINE_NUMBERS		(1<<0)
#define DBG_DISP_ASC_DMP		(1<<1)
#define DBG_DISP_COLORS			(1<<2)

#define DBG_DETAIL_ALL			0xffffffff


/**@}*/





extern uint32_t cw_dbg_opt_display;
extern uint32_t cw_dbg_opt_level;
extern struct cw_StrListElem cw_dbg_strings[];

/*
#define cw_dbg cw_dbg_colored
*/

/*
#define cw_dbg_dmp(type,...) cw_dbg_dmp_(type,__FILE__,__LINE__,__VA_ARGS__)
*/
/*
#define cw_dbg_dmp cw_dbg_dmp_
*/

void cw_dbg_dmp_(int level, const uint8_t * data, int len, const char *format, ...);



extern void cw_dbg_elem(int level, struct conn *conn, int msg, struct cw_ElemHandler * handler,
			 const uint8_t * msgbuf, int len);


void cw_dbg_pkt_nc(int level,struct netconn *nc, uint8_t * packet, int len,struct sockaddr *from);

void cw_dbg_msg(int level,struct conn *conn, uint8_t * packet, int len,struct sockaddr *from);
char * cw_dbg_mkdmp(const uint8_t * data, int len);
void cw_dbg_version_subelem(int level, const char *context, int subtype, 
		uint32_t vendor_id, const uint8_t * vstr, int len);


/**
  * Set debug level
  * @param level debug level to set, allowed values are enumberated in #cw_dbg_levels structure.
  * @param on 1: turns the specified debug level on, 0: turns the specified debug level off.
  */
/*#define cw_dbg_set_level(level,on)\
	(on ? cw_dbg_opt_level |= (1<<(level)) : (cw_dbg_opt_level &= (-1)^(1<<(level))))
*/

/**
 * Check if a specific debug level is set.
 * @param level Level to check
 * @return 0 if leveln is not set, otherwise level is set
 */
#define cw_dbg_is_level(level)\
	(cw_dbg_opt_level & (1<<level))



/**
 *@}
 */
