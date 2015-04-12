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

/**
 * @file
 * @brief Connection object stuff
 */

#ifndef __CONN_H
#define __CONN_H

#include <stdint.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "fragman.h"
#include "cwmsg.h"
#include "cwrmsg.h"
#include "wtpinfo.h"


#include "action.h"

#include "itemstore.h"
#include "intavltree.h"

/**
 * Connection Object
 */ 
struct conn {
	int sock;
	struct sockaddr_storage addr;
	int recv_timeout;

	cw_itemstore_t outgoing;
	cw_itemstore_t incomming;
	cw_itemstore_t remote;
	cw_itemstore_t local;

	
	/** Wireless Binding ID of this connection */
	uint8_t wbid;


	/** Counter for mandatory message elements */
	struct avltree *mand;


	/** Actionsdefs - this defines the possible actions for
	    this conn object, so in the end this is the specification
	    of the protocoll */
	struct cw_actiondef *actions;

	/** Current CAPWAP state */

	uint8_t capwap_state;


	/** The framgent manager used on this connection */
	FRAGMAN *fragman;

	/** Current Fragment ID */
	int fragid;

	int seqnum;

	int last_seqnum_received;
	int last_message_id_received;


	struct cwmsg req_msg;
	struct cwmsg resp_msg;

	/** Buffer for outgoing request messages */
	uint8_t req_buffer[65536];

	/** Buffer for outgoing response messages */
	uint8_t resp_buffer[65536];
	
		

	/** Max mtu, could be changed during discovery */
	int mtu;
	/* 0 mtu discovery turned off, 1 discovery tuned on */
	int mtu_discovery;	

	/* receive and send methods */

	int (*recv_packet) (struct conn *, uint8_t *, int);
	int (*recv_packet_peek) (struct conn *, uint8_t *, int);
	int (*send_packet) (struct conn *, const uint8_t *, int);

	int (*read) (struct conn *, uint8_t *, int);
	int (*write) (struct conn *, const uint8_t *, int);

	/* optional packet queue */
	uint8_t **q;
	int qsize;
	int qrpos;
	int qwpos;
	sem_t q_sem;
	uint8_t *cur_packet;
	int cur_packet_len;
	int cur_packet_pos;

	/* dtls stuff */
	int (*dtls_start) (struct conn *);
	int (*dtls_accept) (struct conn *);

	char *dtls_psk;
	int dtls_psk_len;

	char *dtls_cert_file;
	char *dtls_key_file;
	char *dtls_key_pass;

	void *dtls_data;
	char *dtls_cipher;
	int dtls_error;

	uint8_t dtls_cookie[8];

	int dtls_verify_peer;

	uint8_t dtls_buffer[2048];
	int dtls_buffer_len;
	int dtls_buffer_pos;

	/* capwap timers */
	int retransmit_interval;
	int max_retransmit;
	int wait_dtls;
	int wait_join;



	/* used to link the conn obj with other objects */
	void *data;

	/* misc */
	int capwap_mode;

	int strict_capwap;

/*
	int (*request_handler) (void *);
	void *request_handler_param;
*/

};



#define conn_is_strict_capwap(conn) (conn->strict_capwap)



struct conn *conn_create(int sock, struct sockaddr *addr, int qsize);
struct conn *conn_create_noq(int sock, struct sockaddr *addr);


extern int conn_send_cwmsg(struct conn *conn, struct cwmsg *cwmsg);
//extern int conn_process_packet(struct conn *conn, uint8_t * packet, int len,
//				int (*cb) (void *, uint8_t *,int len), void *cbarg);

extern uint8_t *conn_get_message(struct conn *conn);

extern int conn_send_packet(struct conn *conn, const uint8_t * buffer, int len);
extern void conn_destroy(struct conn *conn);

uint8_t *conn_q_get_packet(struct conn *conn);
extern int conn_q_recv_packet(struct conn *conn, uint8_t * buffer, int len);
extern int conn_q_recv_packet_peek(struct conn *conn, uint8_t * buffer, int len);

extern int conn_recv_packet(struct conn *conn, uint8_t * buf, int len);
extern int conn_recv_packet_peek(struct conn *conn, uint8_t * buf, int len);

extern int conn_send_response(struct conn *conn, struct cwmsg *cwmsg, int seqnum);
extern struct cwrmsg *conn_get_response(struct conn *conn);


#define conn_get_next_seqnum(conn) (conn->seqnum=((conn->seqnum+1)&0xff))
#define conn_get_last_seqnum(conn) (conn->seqnum&0xff)


/* connlist stuff */
#include "avltree.h"
struct connlist {
/*	struct conn ** connlist; */

	struct avltree *t;
	int len;
	pthread_mutex_t connlist_mutex;
};


struct connlist *connlist_create(int len);
void connlist_lock(struct connlist *cl);
void connlist_unlock(struct connlist *cl);
void conlist_destroy(struct connlist *cl);
struct conn *connlist_get(struct connlist *cl, const struct sockaddr *addr);
struct conn *connlist_add(struct connlist *cl, struct conn *conn);
void connlist_remove(struct connlist *cl, struct conn *conn);
void connlist_destroy(struct connlist *cl);
void conn_q_add_packet(struct conn *conn, uint8_t * packet, int len);

struct image_identifier;
struct cwimage_data;

extern void conn_prepare_request(struct conn *conn, int type);
extern int conn_prepare_image_data_request(struct conn *conn, struct cwimage_data *,
					   struct image_identifier *id);
extern void conn_detect_capwap(struct conn *conn, struct wtpinfo *wtpinfo);
struct cwrmsg *conn_send_request(struct conn *conn);
struct cwrmsg *conn_wait_for_message(struct conn *conn, time_t timer);

struct cwrmsg *conn_wait_for_request(struct conn *conn, int *msglist, time_t timer);


#define conn_is_error(conn) (conn->dtls_error)

void conn_init(struct conn *conn);

extern int cw_read_messages(struct conn *conn);




#endif				/* __CONLIST_H */
