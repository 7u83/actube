#ifndef __DBG_H
#define __DBG_H



void cw_dbg_elem_(int msg, int msgelem, const uint8_t * msgbuf, int len);


#ifdef WITH_CW_LOG_DEBUG

	#define cw_dbg_elem(msgtype,msgelemtype,msgbuf,msglen) cw_dbg_elem_(msgtype,msgelemtype,msgbuf,msglen)

#else

	#define cw_dbg_elem(...)
#endif

#endif
