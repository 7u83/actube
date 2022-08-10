/*
 * Copyright 2013,2020, The PLANIX Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <errno.h>


#include "conn.h"
#include "cw.h"
#include "log.h"
#include "dbg.h"
#include "sock.h"
#include "timer.h"




int conn_send_data_msg(struct cw_Conn * conn, uint8_t *rawmsg,int len)
{
	int packetlen = len;
	int fragoffset;
	int hlen;
	
	uint8_t * ptr;
	int mtu;
	
	cw_dbg_msg(DBG_MSG_OUT, conn,rawmsg, packetlen,(struct sockaddr*)&conn->addr);


	ptr = rawmsg;

	fragoffset = 0;

	hlen = cw_get_hdr_hlen(rawmsg)*4;

	mtu = conn->mtu;

	while (packetlen>mtu){
		cw_set_hdr_flags(rawmsg,CAPWAP_FLAG_HDR_F,1);
		cw_set_dword(ptr+4, conn->fragid<<16 | fragoffset<<3 );

		cw_dbg_pkt(DBG_PKT_OUT,conn,ptr,mtu,(struct sockaddr*)&conn->addr);

/*		if (conn->write_data(conn,ptr,mtu)<0)*/
			return -1;

/*		// XXX Fragmentation stuff..*/
		ptr +=mtu-hlen;
		fragoffset+=(mtu-hlen)/8;

		packetlen-=mtu-hlen;

	}


	if (fragoffset)
		cw_set_hdr_flags(rawmsg,CAPWAP_FLAG_HDR_F | CAPWAP_FLAG_HDR_L,1);
	else
		cw_set_hdr_flags(rawmsg,CAPWAP_FLAG_HDR_F,0);

	cw_set_dword(ptr+4, conn->fragid<<16 | fragoffset<<3 );


	cw_dbg_pkt(DBG_PKT_OUT,conn,ptr,packetlen,(struct sockaddr*)&conn->addr);

/*//	return conn->write_data(conn,ptr,packetlen-0);*/

	return 0;
}


#define MAX_MTU 9500
int 
cw_send_msg( struct cw_Conn * conn, uint8_t *msg)
{
	uint8_t buf[MAX_MTU];
	int fragoffset,hlen,mtu;
	int packetlen, msglen;

	mtu = conn->mtu;

	/* align mtu to 8 */
	mtu &= ~3;

	/* zero the first 8 bytes */
	cw_set_dword(buf + 0, 0);
	cw_set_dword(buf + 4, 0);

	/* unencrypted */
	cw_set_hdr_preamble(buf, CAPWAP_VERSION << 4 | 0);

	/* set rmac, wireless binding id, and radio id */
	cw_set_hdr_rmac(buf, conn->base_rmac);
	cw_set_hdr_wbid(buf, conn->wbid);
	cw_set_hdr_rid(buf, 0);


	fragoffset = 0;

	hlen = cw_get_hdr_hlen(buf)*4;

	packetlen = hlen + cw_get_msg_elems_len(msg) + 8;

	msglen = cw_get_msg_elems_len(msg) + 8;


	while (msglen + hlen > mtu){
		memcpy(buf+hlen,msg+(fragoffset*8),mtu-hlen);
		msglen -= (mtu - hlen);

		cw_set_hdr_flags(buf,CAPWAP_FLAG_HDR_F,1);
		cw_set_dword(buf+4, conn->fragid<<16 | fragoffset<<3 );

		cw_dbg_pkt(DBG_PKT_OUT,conn,buf,mtu,(struct sockaddr*)&conn->addr);
		if (conn->write(conn,buf,mtu)<0)
			return -1;

		fragoffset+=(mtu-hlen)/8;
		cw_set_hdr_flags(buf,CAPWAP_FLAG_HDR_M,0);
		cw_set_hdr_flags(buf,CAPWAP_FLAG_HDR_W,0);
		hlen = 8;
		cw_set_hdr_hlen(buf,hlen/4);

	}

	if (fragoffset)
		cw_set_hdr_flags(buf,CAPWAP_FLAG_HDR_F | CAPWAP_FLAG_HDR_L,1);
	else
		cw_set_hdr_flags(buf,CAPWAP_FLAG_HDR_F,0);

	memcpy(buf+hlen,msg+(fragoffset*8),mtu-hlen);

	cw_set_dword(buf+4, conn->fragid<<16 | fragoffset<<3 );


	cw_dbg_pkt(DBG_PKT_OUT,conn,buf,msglen+hlen,(struct sockaddr*)&conn->addr);

	return conn->write(conn,buf,msglen + hlen);
	
}






int cw_send_request(struct cw_Conn *conn,int msg_id)
{
	time_t timer;
	int i;
	int rc;
	char sock_buf[SOCK_ADDR_BUFSIZE];
	cw_init_request(conn, msg_id);
	if ( cw_assemble_message(conn, conn->req_buffer) == -1 ){
		errno=ENOMSG;
		return -1;
	}
/*	conn_send_msg(conn, conn->req_buffer);*/

	

	rc=-1;
	for (i=0; i<conn->max_retransmit && rc<0; i++){
		conn_send_msg(conn, conn->req_buffer);
		if ( i>0 ){
			cw_log(LOG_WARNING,"Retransmitting request ... %d",i);
		}
		timer = cw_timer_start(conn->retransmit_interval);
		while (!cw_timer_timeout(timer) && rc<0){
	
        	        rc =cw_read_messages(conn);
			if(rc<0){
				if (errno!=EAGAIN)
					break;
			}

		}
		if (rc<0){
			if(errno!=EAGAIN)
				break;
			
		}

	}

	if ( rc <0 && errno != EAGAIN) {
		cw_log(LOG_ERR,"Can't read from %s: %s",sock_addr2str(&conn->addr,sock_buf),strerror(errno));
	}
	if ( rc <0 && errno == EAGAIN) {
		errno=ETIMEDOUT;
		rc=-1;
	}

	return rc;
}


/*// XXX find a better name for this function */
int cw_send_custom_request_2(struct cw_Conn *conn,int msg_id)
{




	return 0;
}

