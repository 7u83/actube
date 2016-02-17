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


#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "capwap.h"
#include "dbg.h"

#include "log.h"
#include "cw_util.h"

#include "conn.h"
#include "sock.h"


int conn_send_msg(struct conn *conn, uint8_t * rawmsg);




/**
 * Init response message header
 */
void cw_init_response(struct conn *conn, uint8_t * req)
{
	uint8_t *buffer = conn->resp_buffer;
	int shbytes = cw_get_hdr_msg_offset(req);
	int dhbytes;
	memcpy(buffer, req, shbytes);


	cw_set_hdr_rmac(buffer,conn->base_rmac);
//	cw_set_hdr_hlen(buffer, 2);
//	cw_set_hdr_flags(buffer, CW_FLAG_HDR_M, 1);



	dhbytes = cw_get_hdr_msg_offset(buffer);

	uint8_t *msgptr = req + shbytes;
	uint8_t *dmsgptr = buffer + dhbytes;

	cw_set_msg_type(dmsgptr, cw_get_msg_type(msgptr) + 1);
	cw_set_msg_seqnum(dmsgptr, cw_get_msg_seqnum(msgptr));
	cw_set_msg_flags(dmsgptr, 0);
}

void cw_init_request(struct conn *conn, int msg_id)
{
	uint8_t *buffer = conn->req_buffer;

	/* zero the first 8 bytes */
	cw_put_dword(buffer + 0, 0);
	cw_put_dword(buffer + 4, 0);

	/* unencrypted */
	cw_set_hdr_preamble(buffer, CAPWAP_VERSION << 4 | 0);
	
	cw_set_hdr_rmac(buffer,conn->base_rmac);
	//cw_set_hdr_hlen(buffer, 2);


	cw_set_hdr_wbid(buffer, 1);
	cw_set_hdr_rid(buffer, 0);


	uint8_t *msgptr = cw_get_hdr_msg_offset(buffer) + buffer;
	cw_set_msg_type(msgptr, msg_id);
	cw_set_msg_flags(msgptr, 0);
	cw_set_msg_elems_len(msgptr, 0);


}

/**
 * send a response 
 */
int cw_send_response(struct conn *conn, uint8_t * rawmsg, int len)
{
	cw_init_response(conn, rawmsg);
	if (cw_put_msg(conn, conn->resp_buffer) == -1)
		return 0;
	conn_send_msg(conn, conn->resp_buffer);
	return 1;
}




/**
 * Special case error message, which is sent when an unexpected messages 
 * was received or something else happened.
 * @param conn conection
 * @param rawmsg the received request message, which the response belongs to
 * @pqram result_code result code to send
 * @return 1
 */
int cw_send_error_response(struct conn *conn, uint8_t * rawmsg, uint32_t result_code)
{
	cw_init_response(conn, rawmsg);

	uint8_t *out = conn->resp_buffer;

	uint8_t *dst = cw_get_hdr_msg_elems_ptr(out);
	int l = cw_put_elem_result_code(dst, result_code);

	cw_set_msg_elems_len(out + cw_get_hdr_msg_offset(out), l);

	conn_send_msg(conn, conn->resp_buffer);

	return 1;
}


static int process_elements(struct conn *conn, uint8_t * rawmsg, int len,struct sockaddr *from)
{
	struct cw_action_in as, *af, *afm;

 	int offset = cw_get_hdr_msg_offset(rawmsg);

	uint8_t *msg_ptr = rawmsg + offset;

	int elems_len = cw_get_msg_elems_len(msg_ptr);

	int payloadlen=len-offset;
	
	/* pre-check message */
	if (payloadlen-8 !=  elems_len ) {

		if (conn->strict_hdr) {
			cw_dbg(DBG_MSG_ERR,
			       "Discarding message from %s, msgelems len=%d, payload len=%d, (Strict CAPWAP) ",
			       sock_addr2str(&conn->addr), elems_len, payloadlen-8);
			errno=EAGAIN;	
			return -1;
		}
		if (elems_len < payloadlen-8 ) {
			cw_dbg(DBG_RFC,
			       "Packet from from %s has %d bytes of extra data, ignoring.",
			       sock_addr2str(&conn->addr), payloadlen-8 - elems_len);
			elems_len = len - 8;
		}

		if (elems_len > payloadlen-8) {
			cw_dbg(DBG_RFC,
			       "Packet from from %s has msgelems len of %d bytes, but has only %d bytes of data, truncating.",
			       sock_addr2str(&conn->addr), elems_len, payloadlen - 8);
			elems_len=payloadlen-8;
		}
	}



	/* prepare struct for search operation */
	as.capwap_state = conn->capwap_state;
	as.msg_id = cw_get_msg_id(msg_ptr);
	as.vendor_id = 0;
	as.elem_id = 0;
	as.proto = 0;


	/* Search for state/message combination */
	afm = cw_actionlist_in_get(conn->actions->in, &as);

	if (!afm) {
		/* Throw away unexpected response messages */
		if (!(as.msg_id & 1)) {
			cw_dbg(DBG_MSG_ERR,
			       "Message type %d (%s) unexpected/illigal in %s State, discarding.",
			       as.msg_id, cw_strmsg(as.msg_id),
			       cw_strstate(conn->capwap_state));
			errno = EAGAIN;
			return -1;
		}

		/* Request message not found in current state, check if we know 
		   anything else about this message type */
		const char *str = cw_strheap_get(conn->actions->strmsg, as.msg_id);
		int result_code = 0;
		if (str) {
			/* Message found, but it was in wrong state */
			cw_dbg(DBG_MSG_ERR,
			       "Message type %d (%s) not allowed in %s State.", as.msg_id,
			       cw_strmsg(as.msg_id), cw_strstate(as.capwap_state));
			result_code = CW_RESULT_MSG_INVALID_IN_CURRENT_STATE;
		} else {
			/* Message is unknown */
			cw_dbg(DBG_MSG_ERR, "Message type %d (%s) unknown.",
			       as.msg_id, cw_strmsg(as.msg_id),
			       cw_strstate(as.capwap_state));
			result_code = CW_RESULT_MSG_UNRECOGNIZED;

		}
		cw_send_error_response(conn, rawmsg, result_code);
		errno = EAGAIN;
		return -1;
	}


	/* Execute start processor for message */
	if (afm->start) {
		afm->start(conn, afm, rawmsg, len,from);
	}

	uint8_t *elems_ptr = cw_get_msg_elems_ptr(msg_ptr);
	uint8_t *elem;

	/* Create an avltree to catch the found mandatory elements */
	conn->mand = intavltree_create();

	/* iterate through message elements */
	cw_foreach_elem(elem, elems_ptr, elems_len) {

		as.elem_id = cw_get_elem_id(elem);
		int elem_len = cw_get_elem_len(elem);

		cw_dbg_elem(DBG_ELEM, conn, as.msg_id, as.elem_id, cw_get_elem_data(elem),
			    elem_len);


		af = cw_actionlist_in_get(conn->actions->in, &as);

		if (!af) {
			cw_dbg(DBG_ELEM_ERR,
			       "Element %d (%s) not allowed in msg of type %d (%s), ignoring.",
			       as.elem_id, cw_strelemp(conn->actions,as.elem_id), as.msg_id,
			       cw_strmsg(as.msg_id));
			continue;
		}

		int afrc = 1;
		if (af->start) {
			afrc = af->start(conn, af, cw_get_elem_data(elem), elem_len,from);

		}

		if (af->mand && afrc) {
			/* add found mandatory message element 
			   to mand list */
			intavltree_add(conn->mand, (intptr_t)af->item_id);
		}

	}

	/* all message elements are processed, do now after processing
	   by calling the "end" function for the message */

	int result_code = 0;
	if (afm->end) {
		result_code = afm->end(conn, afm, rawmsg, len,from);
	}

	/* if we've got a request message, we always have to send a response message */
	if (as.msg_id & 1) {
		if (result_code > 0) {
			/* the end method gave us an result code>0, so
			   send an error message */
			cw_send_error_response(conn, rawmsg, result_code);
		} else if ( result_code == 0 ){
			/* All ok, send regular response message */
			cw_send_response(conn, rawmsg, len);
		} else {
			/* the request message is ignored, no response
			   will be sent */
			errno=EAGAIN;	
		}
	} else {
		/* 
		 * Whe have got a response message.
		 * Put further actions here, if needed.
		 */
	}

	intavltree_destroy(conn->mand);

	return result_code;

}








static int process_message(struct conn *conn, uint8_t * rawmsg, int rawlen,struct sockaddr *from)
{
	uint8_t *msgptr = rawmsg + cw_get_hdr_msg_offset(rawmsg);


	uint32_t type = cw_get_msg_type(msgptr);

	if (!(type & 0x1)) {
		/* It's a response  message, no further examination required. */
		return process_elements(conn, rawmsg, rawlen, from);
	}

	/* It's a request message, check if seqnum is right and if
	 * we have already sent a response message*/

	uint8_t seqnum = cw_get_msg_seqnum(msgptr);

	int s1 = conn->last_seqnum_received;
	int s2 = seqnum;
	int sd = s2 - s1;

	if ((sd > 0 && sd < 128) || (sd < 0 && sd < -128) || s1 < 0) {
		/* seqnum is ok, normal message processing */
		conn->last_seqnum_received = seqnum;
		return process_elements(conn, rawmsg, rawlen,from);
	}

	if (sd != 0) {
		cw_dbg(DBG_MSG_ERR,
		       "Discarding message from %s, old seqnum, seqnum = %d, last seqnum=%d",
		       sock_addr2str(&conn->addr), s2, s1);
		errno = EAGAIN;
		return -1;
	}

	/* the received request message was retransmittet by our peer,
	 * let's retransmit our response message if we have one*/

	cw_dbg(DBG_MSG_ERR,
	       "Retransmitted request message from %s detected, seqnum=%d, type=%d",
	       sock_addr2str(&conn->addr), s2, type);


	if (cw_get_hdr_msg_type(conn->resp_buffer) - 1 != type) {
		cw_dbg(DBG_MSG_ERR,
		       "No cached response for retransmission, request seqnum=%d,in cache=%d",
		       s2, conn->resp_msg.type);
		errno = EAGAIN;
		return -1;
	}

	cw_dbg(DBG_MSG_ERR, "Retransmitting response message to %s, seqnum=%d",
	       sock_addr2str(&conn->addr), s2);

	// XXX untested
	conn_send_msg(conn, conn->resp_buffer);
	errno = EAGAIN;
	return -1;
}


/*
 * Process an incomming CAPWAP packet, assuming the packet is already decrypted
 * @param conn conection object 
 * @param packet pointer to packet data
 * @param len lenght of packet data
 */
int conn_process_packet(struct conn *conn, uint8_t * packet, int len,struct sockaddr *from)
{
	/* show this packet in debug output */
	cw_dbg_pkt(DBG_PKT_IN, conn, packet, len,from);


	if (len < 8) {
		/* packet too short */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, packet too short, len=%d,  at least 8 expected.",
		       sock_addr2str(&conn->addr), len);
		errno = EAGAIN;
		return -1;
	}

	int preamble = cw_get_hdr_preamble(packet);

	if ((preamble & 0xf0) != (CAPWAP_VERSION << 4)) {
		/* wrong version */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, wrong version, version=%d, version %d expected.",
		       sock_addr2str(&conn->addr), (preamble & 0xf0) >> 4,
		       CAPWAP_VERSION);
		errno = EAGAIN;
		return -1;
	}

	if (preamble & 0xf) {
		/* Encrypted data, this shuold never happen here */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, encrypted data after decryption ...",
		       sock_addr2str(&conn->addr));
		errno = EAGAIN;
		return -1;
	}


	int offs = cw_get_hdr_msg_offset(packet);


	int payloadlen = len - offs;
	if (payloadlen < 0) {
		/* Eleminate messages with wrong header size */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, header length (%d) greater than packet len (%d).",
		       sock_addr2str(&conn->addr), offs, len);
		errno = EAGAIN;
		return -1;
	}

	/* Check if Radio MAC is preset */
	if (cw_get_hdr_flag_m(packet)) {

		if (cw_get_hdr_rmac_len(packet) + 8 > offs) {
			/* wrong rmac size */
			cw_dbg(DBG_PKT_ERR,
			       "Discarding packet from %s, wrong R-MAC size, size=%d",
			       sock_addr2str(&conn->addr), *(packet + 8));
			errno = EAGAIN;
			return -1;
		}

	}


	if (cw_get_hdr_flag_f(packet)) {
		/* fragmented, add the packet to fragman */
		uint8_t *f;
		f = fragman_add(conn->fragman, packet, offs, payloadlen);
		if (f == NULL){
			errno=EAGAIN;
			return -1;
		}


		cw_dbg_pkt(DBG_PKT_IN, conn, f+4, *(uint32_t*)f,from);
		cw_dbg_msg(DBG_MSG_IN, conn, f+4, *(uint32_t*)f,from);

		// XXX: Modify fragman to not throw away CAPWAP headers

		int rc = process_message(conn, f + 4, *(uint32_t *) f, from);

		free(f);
		return rc;
	}

	/* not fragmented, we have a complete message */
	cw_dbg_msg(DBG_MSG_IN, conn, packet, len,from);
	return process_message(conn, packet, len, from);
}


/**
 * Used as main message loop
 */
int cw_read_messages(struct conn *conn)
{
	uint8_t buf[2024];
	int len = 2024;

	int n = conn->read(conn, buf, len);
	if (n < 0)
		return n;

	if (n > 0) {
		return conn_process_packet(conn, buf, n,(struct sockaddr*)&conn->addr);
	}
	errno = EAGAIN;
	return -1;
}

int cw_read_from(struct conn * conn)
{
	if (!conn->readfrom){
		cw_log(LOG_ERR,"Fatal error, no readfrom method available.");
		errno = EPROTO;
		return -1;
	}
	uint8_t buf[2024];
	int len = 2024;

	struct sockaddr_storage from;
	int n = conn->readfrom(conn, buf, len,&from);
	if (n < 0)
		return n;

	if (n > 0) {
		return conn_process_packet(conn, buf, n,(struct sockaddr*)&from);
	}
	errno = EAGAIN;
	return -1;


}
