
#include <stdint.h>
#include <stdio.h>

#include "conn.h"
#include "capwap.h"
#include "capwap_items.h"

#include "sock.h"
#include "action.h"
#include "cw_log.h"
#include "dbg.h"


//int cw_out_msg(struct conn *conn, uint8_t * rawout);

int conn_send_msg(struct conn *conn, uint8_t * rawmsg);

int cw_send_response(struct conn *conn, uint8_t * rawmsg, int len)
{
//	struct priv priv;

	cw_init_response(conn, rawmsg);


//	uint8_t *msgptr = rawmsg + cw_get_hdr_msg_offset(rawmsg);

//	cw_action_out_t as;


	cw_put_msg(conn,conn->resp_buffer);
	conn_send_msg(conn, conn->resp_buffer);
	return 1;

}


int cw_process_msg(struct conn *conn, uint8_t * rawmsg, int len)
{
	struct cw_action_in as, *af,*afm;

	uint8_t *msg_ptr = rawmsg + cw_get_hdr_msg_offset(rawmsg);

	int elems_len = cw_get_msg_elems_len(msg_ptr);
/*
	if (8+elems_len != len){
		cw_dbg(DBG_MSG_ERR,"Discarding message from %s, msgelems len=%d, data len=%d, (strict capwap) ",
			sock_addr2str(&conn->addr),elems_len,len-8);

		if (conn_is_strict_capwap(conn)){
			cw_dbg(DBG_MSG_ERR,"Discarding message from %s, msgelems len=%d, data len=%d, (strict capwap) ",
				sock_addr2str(&conn->addr),elems_len,len-8);
			return 0;
		}
		if (8+elems_len < len){
			cw_dbg(DBG_CW_RFC,"Packet from from %s has %d bytes extra data.",
				sock_addr2str(&conn->addr),len-8-elems_len);
			elems_len=len-8;
		}

		if (8+elems_len > len){
			cw_dbg(DBG_CW_RFC,"Packet from from %s hass msgelems len of %d bytes but has only %d bytes of data, truncating.",
				sock_addr2str(&conn->addr),elems_len,len-8);
		}
		return 1;
	}

*/


	/* prepare struct for search operation */
	as.capwap_state = conn->capwap_state;
	as.msg_id = cw_get_msg_id(msg_ptr);
	as.vendor_id = 0;
	as.elem_id = -1;
	as.proto=0;


	/* Search for state/message combination */
	afm = cw_actionlist_in_get(conn->actions->in, &as);

	/* Check if message comes in right state */
	if (!afm) {
		cw_dbg(DBG_MSG_ERR, "Message type %d (%s) not allowed in %s State.",
		       as.msg_id, cw_strmsg(as.msg_id), cw_strstate(as.capwap_state));
		return 0;
	}


	/* Execute start processor for message */
	if (afm->start) {
		afm->start(conn, afm, rawmsg, len);
	}

	uint8_t *elems_ptr = cw_get_msg_elems_ptr(msg_ptr);
	uint8_t *elem;

	cw_foreach_elem(elem, elems_ptr, elems_len) {

		as.elem_id = cw_get_elem_id(elem);
		int elem_len = cw_get_elem_len(elem);

		cw_dbg_elem(conn,as.msg_id, as.elem_id, cw_get_elem_data(elem), elem_len);


		af = cw_actionlist_in_get(conn->actions->in, &as);

		if (!af) {
			cw_dbg(DBG_ELEM_ERR, "Element %d not allowed in msg %d (%s)",
			       as.elem_id, as.msg_id, cw_strmsg(as.msg_id));
			continue;
		}

		if (af->start) {
			af->start(conn, af, cw_get_elem_data(elem), elem_len);
		}

	}


	cw_send_response(conn, rawmsg, len);

	if (afm->end) {
		afm->end(conn, afm, rawmsg, len);
	}


	return 0;

}
