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
#define CW_DBG_PKT_IN		0x00000001	/* Headers of incoming CAPWAP packets */
#define CW_DBG_PKT_OUT		0x00000002	/* Headers of outgoing CAPWAP packets */



/**@}*/



