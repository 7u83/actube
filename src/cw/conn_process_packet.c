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

#include "cw.h"
#include "dbg.h"

#include "log.h"
#include "cw_util.h"

#include "conn.h"
#include "sock.h"

/*#include "stravltree.h"*/
#include "mod.h"
#include "msgset.h"

#include "ktv.h"






/**
 * Init response message header
 */
void cw_init_response(struct conn *conn, uint8_t * req)
{
	uint8_t *buffer;
	int shbytes, dhbytes;
	uint8_t *msgptr, *dmsgptr;
	
	buffer = conn->resp_buffer;
	shbytes = cw_get_hdr_msg_offset(req);
	
	memcpy(buffer, req, shbytes);

	cw_set_hdr_rmac(buffer, conn->base_rmac);
/*
//      cw_set_hdr_hlen(buffer, 2);
//      cw_set_hdr_flags(buffer, CW_FLAG_HDR_M, 1);
*/

	dhbytes = cw_get_hdr_msg_offset(buffer);

	msgptr = req + shbytes;
	dmsgptr = buffer + dhbytes;

	cw_set_msg_type(dmsgptr, cw_get_msg_type(msgptr) + 1);
	cw_set_msg_seqnum(dmsgptr, cw_get_msg_seqnum(msgptr));
	cw_set_msg_flags(dmsgptr, 0);
}

void cw_init_request(struct conn *conn, int msg_id)
{
	uint8_t *buffer = conn->req_buffer;
	uint8_t *msgptr;
	
	/* zero the first 8 bytes */
	cw_set_dword(buffer + 0, 0);
	cw_set_dword(buffer + 4, 0);

	/* unencrypted */
	cw_set_hdr_preamble(buffer, CAPWAP_VERSION << 4 | 0);

	cw_set_hdr_rmac(buffer, conn->base_rmac);
/*
	//cw_set_hdr_hlen(buffer, 2);
*/


	cw_set_hdr_wbid(buffer, conn->wbid);
	cw_set_hdr_rid(buffer, 0);


	msgptr = cw_get_hdr_msg_offset(buffer) + buffer;
	cw_set_msg_type(msgptr, msg_id);
	cw_set_msg_flags(msgptr, 0);
	cw_set_msg_elems_len(msgptr, 0);
}

void cw_init_data_msg(struct conn *conn)
{
	uint8_t *buffer = conn->req_buffer;
	cw_set_dword(buffer + 0, 0);
	cw_set_dword(buffer + 4, 0);

	/* unencrypted */
	cw_set_hdr_preamble(buffer, CAPWAP_VERSION << 4 | 0);



}


/**
 * send a response 
 */
int cw_send_response(struct conn *conn, uint8_t * rawmsg, int len)
{
	int rc;
	cw_init_response(conn, rawmsg);
	rc = cw_put_msg(conn, conn->resp_buffer);
	if (!cw_result_is_ok(rc))
		return 0;
	conn_send_msg(conn, conn->resp_buffer);
	return 1;
}



/**
 * Special case error message, which is sent when an unexpected messages 
 * was received or something else happened.
 * @param conn conection
 * @param rawmsg the received request message, which the response belongs to
 * @param result_code result code to send
 * @return 1
 */
int cw_send_error_response(struct conn *conn, uint8_t * rawmsg, uint32_t result_code)
{
	uint8_t *out,*dst;
	int l;
	
	cw_init_response(conn, rawmsg);

	out = conn->resp_buffer;

	dst = cw_get_hdr_msg_elems_ptr(out);
	l = cw_put_elem_result_code(dst, result_code);

	cw_set_msg_elems_len(out + cw_get_hdr_msg_offset(out), l);

	conn_send_msg(conn, conn->resp_buffer);

	return 1;
}


static struct cw_MsgSet *load_msg_set(struct conn *conn, uint8_t * rawmsg, int len,
				      int elems_len, struct sockaddr *from)
{
	char sock_buf[SOCK_ADDR_BUFSIZE];
	struct cw_Mod *cmod, *bmod;
	
	cmod =
	    cw_mod_detect(conn, rawmsg, len, elems_len, from, CW_MOD_MODE_CAPWAP);
	if (cmod == MOD_NULL) {
		cw_dbg(DBG_MSG_ERR,
		       "Can't find mod to handle connection from %s, discarding message",
		       sock_addr2str_p(from,sock_buf));
		return NULL;
	}

	bmod = 
	    cw_mod_detect(conn, rawmsg, len, elems_len, from, CW_MOD_MODE_BINDINGS);

	
	cw_dbg(DBG_INFO, "Mods deteced: %s,%s", cmod->name, bmod->name);

	conn->cmod = cmod;
	conn->bmod = bmod;

	return cw_mod_get_msg_set(conn,cmod,bmod);

}

/*
int cw_in_check_generic(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len,struct sockaddr *from)
{
//	if (cw_is_request(a->msg_id)){
//		return cw_in_check_generic_req(conn,a,data,len,from);
//	}
//	return cw_in_check_generic_resp(conn,a,data,len,from);

	
}
*/

/*
void cw_read_elem(struct cw_ElemHandler * handler, struct conn * conn, 
		uint8_t * elem_data, int elem_len, struct sockaddr * from){
	mavldata_t data, *result;
	char str[30];
		
	result = handler->type->get(&data,elem_data,elem_len);
	
	handler->type->to_str(result,str,30);
	printf("Read %d-%s: %s %s\n", handler->id, handler->name, handler->key, str);
	//mavl_insert(conn->remote_cfg
}
*/


static int process_elements(struct conn *conn, uint8_t * rawmsg, int len,
			    struct sockaddr *from)
{
	mavl_t mand_found;
	mlist_t unrecognized;
	struct cw_MsgData search;
	struct cw_MsgData * message;
	int result_code;
	cw_State_t *ui;

	uint8_t *elems_ptr;
	uint8_t *elem;
	
	char sock_buf[SOCK_ADDR_BUFSIZE]; /**< to hold str from sockaddr2str */
	
	/*struct cw_action_in as, *af, *afm;*/

	int offset = cw_get_hdr_msg_offset(rawmsg);

	uint8_t *msg_ptr = rawmsg + offset;

	int elems_len = cw_get_msg_elems_len(msg_ptr);

	int payloadlen = len - offset;


	/* pre-check message */
	if (payloadlen - 8 != elems_len) {

		if (conn->strict_hdr) {
			cw_dbg(DBG_MSG_ERR,
			       "Discarding message from %s, msgelems len=%d, payload len=%d, (Strict CAPWAP) ",
			       sock_addr2str(&conn->addr,sock_buf), elems_len, payloadlen - 8);
			errno = EAGAIN;
			return -1;
		}


		if (elems_len < payloadlen - 8) {
			cw_dbg(DBG_RFC,
			       "Packet from from %s has %d bytes of extra data, ignoring.",
			       sock_addr2str(&conn->addr,sock_buf), payloadlen - 8 - elems_len);
			elems_len = len - 8;
		}

		if (elems_len > payloadlen - 8) {

			cw_dbg(DBG_RFC,
			       "Packet from from %s has msgelems len of %d bytes, but has only %d bytes of data, truncating.",
			       sock_addr2str(&conn->addr, sock_buf), elems_len, payloadlen - 8);
			elems_len = payloadlen - 8;
		}
	}


	/* Detect the connecting AC type */
	if (!conn->detected) {

		struct cw_MsgSet *set = load_msg_set(conn, rawmsg, len, elems_len, from);
		if (!set) {
/*
			//cw_log(LOG_ERR, "Error");
*/ 
			errno = EAGAIN;
			return -1;
		}
		conn->cmod->setup_cfg(conn);
		conn->msgset= set;
		conn->detected = 1;
	}

	/** debug the received message */
	cw_dbg_msg(DBG_MSG_IN, conn, rawmsg, len, from);

	/* prepare struct for search operation */
	search.type = cw_get_msg_id(msg_ptr);
	
	/* Search message */
	
	message = mavl_get(conn->msgset->msgdata,&search);
		
	result_code = 0;
	
	if (!message){
		/* Message is unknown */
		if (search.type & 1){
			cw_dbg(DBG_MSG_ERR, 
				"Message type %d [%s] unrecognized, sending response.",
				search.type, cw_strmsg(search.type),
				cw_strstate(conn->capwap_state));
				
			result_code = CAPWAP_RESULT_MSG_UNRECOGNIZED;
			cw_send_error_response(conn, rawmsg, result_code);
			errno = EAGAIN;
			return -1;
		}
		cw_dbg(DBG_MSG_ERR, 
			"Message type %d [%s] unrecognized, discarding.",
		search.type, cw_strmsg(search.type),
		cw_strstate(conn->capwap_state));
		errno = EAGAIN;
		return -1;

	}

	/* Throw away unexpected messages */
	/* maybe we have to check this too: if (!(message->type & 1)) 
	 * means unexpected response message
	 * */
	if (!(message->receiver & conn->role)) {
		cw_dbg(DBG_MSG_ERR,
		       "Message type %d (%s) unexpected/illegal in %s State, discarding.",
		       search.type, cw_strmsg(search.type),
		       cw_strstate(conn->capwap_state));
		errno = EAGAIN;
		return -1;
	}

	/* Check if current state is in state of message */
	ui = message->states;

	for (ui=message->states; ui->state; ui++){
/*		printf("Comparing %d and %d\n", conn->capwap_state, ui->state);*/
		if(ui->state==conn->capwap_state){
			break;
		}
	}
	
	if (!ui->state){
		/* Message found, but it was in wrong state */
		cw_dbg(DBG_MSG_ERR,
		       "Message type %d (%s) not allowed in %s State, sending response.", 
		       search.type,cw_strmsg(search.type), 
		       cw_strstate(conn->capwap_state));
		result_code = CAPWAP_RESULT_MSG_INVALID_IN_CURRENT_STATE;
		cw_send_error_response(conn, rawmsg, result_code);
		errno = EAGAIN;
		return -1;
	}


	
/*	if (conn->msg_start){
		conn->msg_start(conn, afm, rawmsg, len, from);
	}
*/
	/* Execute start processor for message */
/*
//	if (afm->start) {
//		afm->start(conn, afm, rawmsg, len, from);
//	}
*/


	elems_ptr = cw_get_msg_elems_ptr(msg_ptr);


	/* Create an avltree to catch the found mandatory elements */
/*	//conn->mand = stravltree_create();*/

	mand_found = mavl_create_conststr();
	unrecognized = mlist_create(NULL,NULL,sizeof(uint8_t*));

	cw_dbg(DBG_MSG_PARSING ,"*** Parsing message of type %d - (%s) ***",
			message->type,message->name);
	/* iterate through message elements */
	cw_foreach_elem(elem, elems_ptr, elems_len) {
		int rc;

		struct cw_ElemHandlerParams params;
		int elem_len, elem_id, max_len;
		uint8_t * elem_data;


		elem_len = cw_get_elem_len(elem);
		elem_data=cw_get_elem_data(elem);
		elem_id = cw_get_elem_id(elem);
		
		max_len=elems_len-(elem_data-elems_ptr);
		if (elem_len > max_len){
			cw_dbg(DBG_RFC,
			"Messag element claims size of %d bytes, but only %d bytes are left in the payload, truncating.",
			elem_len,max_len-4);
		}
	

		
/*		params.conn=conn;*/

                params.local_cfg=conn->local_cfg;
                params.remote_cfg=conn->remote_cfg;
                params.default_cfg=conn->default_cfg;
                params.global_cfg=conn->global_cfg;
                params.msgset=conn->msgset;


		params.from=from;
		params.msgdata=message;
		params.mand_found=mand_found;
		
		rc = cw_process_element(&params,0,0,elem_id,elem_data,elem_len); 
		

		if (cw_result_is_ok(rc))
			continue;
		
		if (rc == CAPWAP_RESULT_UNRECOGNIZED_MESSAGE_ELEMENT){
			mlist_append(unrecognized,&elem);
			continue;
		}
		
		if (rc < 0 ){
			continue;
		}

	}

	/* all message elements are processed, do now after processing
	   by calling the "end" function for the message */
	   
	cw_check_missing_mand(message,mand_found);
	
	cw_dbg(DBG_MSG_PARSING," *** Done parsing message of type %d (%s) ***", 
				message->type,message->name);
	
	mavl_destroy(mand_found);
	
/*	cw_dbg_ktv_dump(conn->remote_cfg,DBG_CFG_DMP,
		" *** Remote CFG dump ***", "CFG:", " *** End of remote CFG dump");
*/


	if (message->postprocess){
		message->postprocess(conn);
	}


	/* if we've got a request message, we always have to send a response message */
	if (message->type & 1) {
		if (result_code > 0) {
			/* the end method gave us an result code>0, so
			   send an error message */

			cw_send_error_response(conn, rawmsg, result_code);
		} else if (result_code == 0) {
			cw_ktv_set_dword(conn->local_cfg,"result-code",result_code);
			if (ui->next){
				conn->capwap_prevstate = conn->capwap_state;
				conn->capwap_state = ui->next;
			}
				
			/* All is ok, send regular response message */
			cw_send_response(conn, rawmsg, len);
		} else {
			/* the request message is ignored, no response
			   will be sent */
			errno = EAGAIN;
		}
	} else {
		/* 
		 * Whe have got a response message.
		 * Put further actions here, if needed.
		 */
	}

	/*stravltree_destroy(conn->mand);*/

	return result_code;

}






int process_message(struct conn *conn, uint8_t * rawmsg, int rawlen,
		    struct sockaddr *from)
{
	char sock_buf[SOCK_ADDR_BUFSIZE];
	uint8_t seqnum;
	int s1,s2,sd;

	uint8_t *msgptr = rawmsg + cw_get_hdr_msg_offset(rawmsg);


	uint32_t type = cw_get_msg_type(msgptr);

	if (!(type & 0x1)) {
		/* It's a response  message, no further examination required. */
		return process_elements(conn, rawmsg, rawlen, from);
	}

	/* It's a request message, check if seqnum is right and if
	 * we have already sent a response message*/

	seqnum = cw_get_msg_seqnum(msgptr);

	s1 = conn->last_seqnum_received;
	s2 = seqnum;
	sd = s2 - s1;

	if ((sd > 0 && sd < 128) || (sd < 0 && sd < -128) || s1 < 0) {
		/* seqnum is ok, normal message processing */
		conn->last_seqnum_received = seqnum;
		return process_elements(conn, rawmsg, rawlen, from);
	}

	if (sd != 0) {
		cw_dbg(DBG_MSG_ERR,
		       "Discarding message from %s, old seqnum, seqnum = %d, last seqnum=%d",
		       sock_addr2str(&conn->addr,sock_buf), s2, s1);
		errno = EAGAIN;
		return -1;
	}

	/* the received request message was retransmittet by our peer,
	 * let's retransmit our response message if we have one*/


	cw_dbg(DBG_MSG_ERR,
	       "Retransmitted request message from %s detected, seqnum=%d, type=%d",
	       sock_addr2str(&conn->addr,sock_buf), s2, type);


	if (cw_get_hdr_msg_type(conn->resp_buffer) - 1 != type) {
		cw_dbg(DBG_MSG_ERR,
		       "No cached response for retransmission, request seqnum=%d,in cache=%d",
		       s2, conn->resp_msg.type);
		errno = EAGAIN;
		return -1;
	}

	cw_dbg(DBG_MSG_ERR, "Retransmitting response message to %s, seqnum=%d",
	       sock_addr2str(&conn->addr, sock_buf), s2);

	/*// XXX untested*/
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
int conn_process_packet2(struct conn *conn, uint8_t * packet, int len,
			struct sockaddr *from)
{
	char sock_buf[SOCK_ADDR_BUFSIZE];
	int preamble;
	int offs;
	int payloadlen;
	
	if (len < 8) {
		/* packet too short */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, packet too short, len=%d,  at least 8 expected.",
		       sock_addr2str(&conn->addr, sock_buf), len);
		errno = EAGAIN;
		return -1;
	}

	preamble = cw_get_hdr_preamble(packet);

	if ((preamble & 0xf0) != (CAPWAP_VERSION << 4)) {
		/* wrong version */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, wrong version, version=%d, version %d expected.",
		       sock_addr2str(&conn->addr,sock_buf), (preamble & 0xf0) >> 4,
		       CAPWAP_VERSION);
		errno = EAGAIN;
		return -1;
	}

	if (preamble & 0xf) {
		/* Encrypted data, this shuold never happen here */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, encrypted data after decryption ...",
		       sock_addr2str(&conn->addr,sock_buf));
		errno = EAGAIN;
		return -1;
	}


	offs = cw_get_hdr_msg_offset(packet);


	payloadlen = len - offs;
	if (payloadlen < 0) {
		/* Eleminate messages with wrong header size */
		cw_dbg(DBG_PKT_ERR,
		       "Discarding packet from %s, header length (%d) greater than packet len (%d).",
		       sock_addr2str(&conn->addr,sock_buf), offs, len);
		errno = EAGAIN;
		return -1;
	}

	/* Check if Radio MAC is present */
	if (cw_get_hdr_flag_m(packet)) {

		if (cw_get_hdr_rmac_len(packet) + 8 > offs) {
			/* wrong rmac size */
			cw_dbg(DBG_PKT_ERR,
			       "Discarding packet from %s, wrong R-MAC size, size=%d",
			       sock_addr2str(&conn->addr,sock_buf), *(packet + 8));
			errno = EAGAIN;
			return -1;
		}

	}


	if (cw_get_hdr_flag_f(packet)) {
		/* fragmented, add the packet to fragman */
		uint8_t *f;
		int rc;
		
		f = fragman_add(conn->fragman, packet, offs, payloadlen);
		if (f == NULL) {
			errno = EAGAIN;
			return -1;
		}


		cw_dbg_pkt(DBG_PKT_IN, conn, f + 4, *(uint32_t *) f, from);
/*//		cw_dbg_msg(DBG_MSG_IN, conn, f + 4, *(uint32_t *) f, from);*/

/*		// XXX: Modify fragman to not throw away CAPWAP headers*/

		rc = conn->process_message(conn, f + 4, *(uint32_t *) f, from);

		free(f);
		return rc;
	}

	/* not fragmented, we have a complete message */
/*//	cw_dbg_msg(DBG_MSG_IN, conn, packet, len, from);*/
	return conn->process_message(conn, packet, len, from);
}

int conn_process_packet(struct conn *conn, uint8_t * packet, int len,
			struct sockaddr *from){

	/* show this packet in debug output */
	cw_dbg_pkt(DBG_PKT_IN, conn, packet, len, from);

	return conn_process_packet2(conn,packet,len,from);
}


int conn_process_data_packet(struct conn *conn, uint8_t * packet, int len,
			struct sockaddr *from){

	/* show this packet in debug output */
	cw_dbg_pkt(DBG_PKT_IN, conn, packet, len, from);

	return conn_process_packet2(conn,packet,len,from);
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
		return conn->process_packet(conn, buf, n,
					    (struct sockaddr *) &conn->addr);
	}
	errno = EAGAIN;
	return -1;
}

