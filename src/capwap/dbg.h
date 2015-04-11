#ifndef __DBG_H
#define __DBG_H

#include "conn.h"

#ifndef CW_LOG_DUMP_ROW_LEN
#define CW_LOG_DUMP_ROW_LEN 32
#endif

#ifndef CW_LOG_DUMP_ROW_TAB_LEN
#define CW_LOG_DUMP_ROW_TAB_LEN 8
#endif


void cw_dbg_elem_(struct conn * conn, int msg, int msgelem, const uint8_t * msgbuf, int len);
void cw_dbg_missing_mand(int level,struct conn *conn,cw_action_in_t ** ml,int n,cw_action_in_t *a);
void cw_dbg_packet(struct conn *conn, uint8_t * packet, int len);


#ifdef WITH_CW_LOG_DEBUG

	#define cw_dbg_elem(conn,msgtype,msgelemtype,msgbuf,msglen)\
		 cw_dbg_elem_(conn,msgtype,msgelemtype,msgbuf,msglen)

#else

	#define cw_dbg_elem(...)
#endif

#endif


/** 
 * @defgroup DebugOptions Dbug Options
 * @{
 */


#define DBG_PKT_IN			0x00000001	/* Headers of incoming CAPWAP packets */
#define DBG_PKT_OUT			0x00000002	/* Headers of outgoing CAPWAP packets */
#define DBG_PKT_ERR			0x00000004	/* Error of packets */
#define DBG_PKT_DMP			0x00000008	/* Dump packts */


#define DBG_MSG_IN			0x00000010	/* Parsed CAPWAP/LWAPP messages */
#define DBG_MSG_OUT			0x00000020	/* Parsed CAPWAP/LWAPP messages */

#define DBG_ELEM			0x00000000	/* Show message elements */
#define DBG_ELEM_DMP			0x00000000	/* Dump message elements */

#define DBG_INFO			0x00000000
#define DBG_RFC				0x00000080	/* RCF-realted CAPWAP errors */


#define DBG_MSG_ERR			0x00000000	/* Errors in CAPWAP messages */
#define DBG_ELEM_ERR			0x00000000

/* driver specific debugs */
#define DBG_DRV				0x00010000
#define DBG_DRV_ERR			0x00020000

/* DTLS debugs */
#define DBG_DTLS			0x10000000
#define DBG_DTLS_DETAIL			0x20000000
#define DBG_DTLS_BIO			0x40000000
#define DBG_DTLS_BIO_DMP		0x80000000

#define DBG_ALL				0xffffffff
#define DBG_PKT_INOUT			(DBG_CW_PKT_IN | DBG_CW_PKT_OUT)


#define DBG_DISP_LINE_NUMBERS		0x00000001
#define DBG_DISP_ASC_DMP		0x00000002
#define DBG_DISP_COLORS			0x00000004


#define DBG_DETAIL_ALL			0xffffffff
#define DBG_ERR				(DBG_MSG_ERR | DBG_CW_PKT_ERR)

/**@}*/





extern uint32_t cw_dbg_opt_display;
extern uint32_t cw_dbg_opt_level;
extern struct cw_str cw_dbg_strings[];


/**@}*/


#define cw_dbg(type,...) cw_dbg_colored(type,__FILE__,__LINE__,__VA_ARGS__)
#define cw_dbg_dmp(type,...) cw_dbg_dmp_(type,__FILE__,__LINE__,__VA_ARGS__)


void cw_dbg_colored(int level, const char *file, int line, const char *format, ...);
void cw_dbg_dmp_(int level, const char *file, int line,
		     const uint8_t * data, int len, const char *format, ...);


extern void cw_dbg_elem_colored(int level, struct conn *conn, int msg, int msgelem,
			 const uint8_t * msgbuf, int len);

void cw_dbg_pkt(int level,struct conn *conn, uint8_t * packet, int len);
void cw_dbg_msg(int level,struct conn *conn, uint8_t * packet, int len);




