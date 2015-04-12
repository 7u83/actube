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


#define DBGX(f,...) cw_dbg(DBG_X,f,__VA_ARGS__)  //cw_dbg(DBG_X, f ,__VA_ARGS__)
//#define DBGX(f,...) cw_dbg(DBG_X, f)
//#define DBGX(f,...) printf("hallo\n")


#ifdef WITH_CW_LOG_DEBUG

	#define cw_dbg_elem(level,conn,msgtype,msgelemtype,msgbuf,msglen)\
		 cw_dbg_elem_colored(level,conn,msgtype,msgelemtype,msgbuf,msglen)

#else

	#define cw_dbg_elem(...)
#endif

#endif


/** 
 * @defgroup DebugOptions Dbug Options
 * @{
 */

/**
 * Debug levels
 */ 
enum debug_levels{
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

	/** Error in msg elements */
	DBG_ELEM_ERR,

	/** Show subelements */
	DBG_SUBELEM,

	/** hex dump elements */	
	DBG_ELEM_DMP,

	/** General infos, like CAPWAP state */
	DBG_INFO,	

	/** RFC related */
	DBG_RFC,

	DBG_DTLS,
	DBG_DTLS_DETAIL,
	DBG_DTLS_BIO,
	DBG_DTLS_BIO_DMP,
	
	DBG_X


};


/* driver specific debugs */

#define DBG_DRV				0x00010000
#define DBG_DRV_ERR			0x00020000

/* DTLS debugs */




#define DBG_DISP_LINE_NUMBERS		(1<<0)
#define DBG_DISP_ASC_DMP		(1<<1)
#define DBG_DISP_COLORS			(1<<2)

#define DBG_DETAIL_ALL			0xffffffff


/**@}*/





extern uint32_t cw_dbg_opt_display;
extern uint32_t cw_dbg_opt_level;
extern struct cw_str cw_dbg_strings[];




#define cw_dbg(type,...) cw_dbg_colored(type,__FILE__,__LINE__,__VA_ARGS__)
#define cw_dbg_dmp(type,...) cw_dbg_dmp_(type,__FILE__,__LINE__,__VA_ARGS__)


void cw_dbg_colored(int level, const char *file, int line, const char *format, ...);
void cw_dbg_dmp_(int level, const char *file, int line,
		     const uint8_t * data, int len, const char *format, ...);


extern void cw_dbg_elem_colored(int level, struct conn *conn, int msg, int msgelem,
			 const uint8_t * msgbuf, int len);

void cw_dbg_pkt(int level,struct conn *conn, uint8_t * packet, int len);
void cw_dbg_msg(int level,struct conn *conn, uint8_t * packet, int len);


#define cw_dbg_set_level(level,on)\
	(on ? cw_dbg_opt_level |= (1<<(level)) : (cw_dbg_opt_level &= (-1)^(1<<(level))))

#define cw_dbg_is_level(level)\
	(cw_dbg_opt_level & (1<<level))




