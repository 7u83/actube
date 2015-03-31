
#include <stdint.h>
#include <stdio.h>

#include "conn.h"
#include "capwap.h"
#include "sock.h"
#include "cw_action.h"
#include "cw_log.h"





int cw_process_msg(struct conn * conn,uint8_t * rawmsg,int len)
{
	struct cw_action as,*af;

	uint8_t * msg_ptr = rawmsg+cw_get_hdr_msg_offset(rawmsg);

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
	as.elem_id=-1;
	as.capwap_state=conn->capwap_state;
	as.msg_id = cw_get_msg_id(msg_ptr);
	as.vendor_id=0;


	/* Search for state/message combination */
	af = cw_actionlist_get(conn->msgtr,&as);

	/* Check if message is comes in right state */
	if ( !af ){
		cw_dbg(DBG_MSG_ERR,"Message type %d (%s) not allowed in %s State.",
			as.msg_id,cw_strmsg(as.msg_id),cw_strstate(as.capwap_state));
		return 0;
	}


	if (af->start){
		af->start(conn,af,rawmsg,len);
	}

	uint8_t * elems_ptr = cw_get_msg_elems_ptr(msg_ptr);

	

	uint8_t *elem;

	cw_foreach_elem(elem,elems_ptr,elems_len) {

		as.elem_id = cw_get_elem_id(elem);
		int elem_len = cw_get_elem_len(elem);



		af = cw_actionlist_get(conn->msgtr,&as);
		
		if (!af) {
			cw_dbg(DBG_ELEM_ERR,"Element %d not aallowed in msg %d (%s)",as.elem_id,as.msg_id,cw_strmsg(as.msg_id));
			continue;
		}

		printf("Elem OK: %d, %d\n",as.elem_id,elem_len);
		if (af->start) {
			af->start(conn,af,cw_get_elem_data(elem),elem_len);
		}

	}


	return 0;

}





