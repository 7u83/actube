#ifndef __DBG_H
#define __DBG_H

#include "conn.h"

void cw_dbg_elem_(struct conn * conn, int msg, int msgelem, const uint8_t * msgbuf, int len);
void cw_dbg_missing_mand(int level,struct conn *conn,cw_action_in_t ** ml,int n,cw_action_in_t *a);


#ifdef WITH_CW_LOG_DEBUG

	#define cw_dbg_elem(conn,msgtype,msgelemtype,msgbuf,msglen) cw_dbg_elem_(conn,msgtype,msgelemtype,msgbuf,msglen)

#else

	#define cw_dbg_elem(...)
#endif

#endif
