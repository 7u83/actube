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



/*#include "action.h"*/

/*#include "mbag.h"*/

#include "intavltree.h"
#include "bstr.h"
#include "msgset.h"

#include "mod.h"

struct cw_action_in;

#define CONN_MAX_MSG_LENGTH 65536



#include "connlist.h"


/**
 * Connection Object
 */ 
struct cw_Conn {
	int sock;
	struct sockaddr_storage addr;

	struct connlist * connlist;


	int data_sock;
	struct sockaddr_storage data_addr;

	struct netconn * data_nc;


	int recv_timeout;
	
	
	mavl_t remote_cfg;
	mavl_t default_cfg;
	mavl_t update_cfg;
	mavl_t local_cfg;
	mavl_t global_cfg;
	
	int (*write_header)(struct cw_ElemHandler * handler, uint8_t * dst, int len);
	int (*header_len)(struct cw_ElemHandler *handler);

/*	mbag_t outgoing;
	mbag_t incomming;
	mbag_t remote;
	mbag_t local;

	mbag_t radios;
	mbag_t radios_upd;
	mbag_t config;
	mbag_t config_upd;
*/

/*	uint8_t session_id[16];*/
	bstr16_t session_id;


	int role;

	/** base_mac */
	bstr_t base_rmac;
	
	/** Wireless Binding ID of this connection */
	uint8_t wbid;


	/** Counter for mandatory message elements */
/*	struct avltree *mand;*/


	/** Actionsdefs - this defines the possible actions for
	    this conn object, so in the end this is the specification
	    of the protocoll */


	struct cw_MsgSet * msgset;


	uint8_t capwap_prevstate;

	/** Current CAPWAP state */

	uint8_t capwap_state;


	/** The framgent manager used on this connection */
	frag_t *fragman;

	/** Current Fragment ID */
	int fragid;

	int seqnum;

	int last_seqnum_received;
	int last_message_id_received;


	struct cwmsg req_msg;
	struct cwmsg resp_msg;

	/** Buffer for outgoing request messages */
	uint8_t req_buffer[CONN_MAX_MSG_LENGTH];

	/** Buffer for outgoing response messages */
	uint8_t resp_buffer[CONN_MAX_MSG_LENGTH];
	
		

	/** Max mtu, could be changed during discovery */
	int mtu;
	/* 0 mtu discovery turned off, 1 discovery tuned on */
	int mtu_discovery;	

	/* receive and send methods */

	int (*recv_packet) (struct cw_Conn*, uint8_t *, int);
	int (*recv_packet_peek) (struct cw_Conn*, uint8_t *, int);
	int (*send_packet) (struct cw_Conn*, const uint8_t *, int);
/*
//	int (*recv_data_packet) (struct cw_Conn*, uint8_t *,int);
//	int (*send_data_packet) (struct cw_Conn*, const uint8_t *, int);
*/	

	int (*readfrom) (struct cw_Conn*, uint8_t *, int, struct sockaddr_storage *);
	int (*read) (struct cw_Conn*, uint8_t *, int);
	int (*write) (struct cw_Conn*, const uint8_t *, int);
/*
//	int (*write_data) (struct cw_Conn*, const uint8_t *, int);
*/
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
	int (*dtls_start) (struct cw_Conn*);
	int (*dtls_accept) (struct cw_Conn*);

	bstr16_t dtls_psk;
	int dtls_psk_enable;

	int dtls_dhbits;
	int (*dtls_get_psk)(struct cw_Conn*,const char *user,uint8_t**psk, int *len);

	struct cw_Mod *cmod, *bmod;

	char *dtls_cert_file;
	char *dtls_key_file;
	char *dtls_key_pass;

	void *dtls_data;
	char *dtls_cipher;
	int dtls_error;

	uint8_t dtls_cookie[8];

	int dtls_verify_peer;
	int dtls_mtu;

	uint8_t dtls_buffer[4096];
	int dtls_buffer_len;
	int dtls_buffer_pos;

	/* capwap timers */
	int retransmit_interval;
	int max_retransmit;
	int wait_dtls;
	int wait_join;



	/* used to link the conn obj with other objects */
	void *data;

	/** Mode */
//	int capwap_mode;
	
	/** CAWAP mode for outgoing messages */
	int capwap_mode_out;


	int strict_capwap;
	int strict_hdr;


	int (*process_packet)(struct cw_Conn*conn, uint8_t * packet, int len,struct sockaddr *from);
	int (*process_message)(struct cw_Conn*conn, uint8_t * rawmsg, int rawlen,
			   struct sockaddr *from);

	
	int detected;
	void  * mods;


	int (*msg_start)(struct cw_Conn*conn,struct cw_action_in *a,uint8_t*data,int len,struct sockaddr *from);
	int (*msg_end)(struct cw_Conn*conn,struct cw_action_in *a,uint8_t*elem,int len,struct sockaddr *from);

	int (*elem_end)(struct cw_Conn*conn,struct cw_action_in *a,int afrc,uint8_t*elem,int len,struct sockaddr *from);

/*
//	void (*actions_registered)(struct cw_Conn*conn);
*/

};
typedef struct cw_Conn cw_Conn_t;



#define conn_is_strict_capwap(conn) (conn->strict_capwap)



struct cw_Conn* cw_conn_create(int sock, struct sockaddr *addr, int qsize);
struct cw_Conn* conn_create_noq(int sock, struct sockaddr *addr);


extern int conn_send_cwmsg(struct cw_Conn*conn, struct cwmsg *cwmsg);

/*
//extern int conn_process_packet(struct cw_Conn*conn, uint8_t * packet, int len,
//				int (*cb) (void *, uint8_t *,int len), void *cbarg);
*/

extern int conn_process_packet(struct cw_Conn*conn, uint8_t * packet, int len,struct sockaddr *from);
extern int process_message(struct cw_Conn*conn, uint8_t * rawmsg, int rawlen,
			   struct sockaddr *from);

extern uint8_t *conn_get_message(struct cw_Conn*conn);

extern int conn_send_packet(struct cw_Conn*conn, const uint8_t * buffer, int len);
extern int conn_send_data_packet(struct cw_Conn* conn, const uint8_t * buffer, int len);

extern void conn_destroy(struct cw_Conn*conn);

uint8_t *conn_q_get_packet(struct cw_Conn*conn);
extern int conn_q_recv_packet(struct cw_Conn*conn, uint8_t * buffer, int len);
extern int conn_q_recv_packet_peek(struct cw_Conn*conn, uint8_t * buffer, int len);

extern int conn_recv_packet(struct cw_Conn*conn, uint8_t * buf, int len);
extern int conn_recv_packet_peek(struct cw_Conn*conn, uint8_t * buf, int len);

extern int conn_send_response(struct cw_Conn*conn, struct cwmsg *cwmsg, int seqnum);
extern struct cwrmsg *conn_get_response(struct cw_Conn*conn);


#define conn_get_next_seqnum(conn) (conn->seqnum=((conn->seqnum+1)&0xff))
#define conn_get_last_seqnum(conn) (conn->seqnum&0xff)


void conn_q_add_packet(struct cw_Conn*conn, uint8_t * packet, int len);

struct image_identifier;
struct cwimage_data;

extern void conn_prepare_request(struct cw_Conn*conn, int type);
extern int conn_prepare_image_data_request(struct cw_Conn*conn, struct cwimage_data *,
					   struct image_identifier *id);
/*
//extern void conn_detect_capwap(struct cw_Conn*conn, struct wtpinfo *wtpinfo);
*/

struct cwrmsg *conn_send_request(struct cw_Conn*conn);
struct cwrmsg *conn_wait_for_message(struct cw_Conn*conn, time_t timer);

struct cwrmsg *conn_wait_for_request(struct cw_Conn*conn, int *msglist, time_t timer);

//int conn_q_wait_packet(struct cw_Conn* conn, int seconds);

#define conn_is_error(conn) (conn->dtls_error)

void cw_conn_init(struct cw_Conn*conn);

extern int cw_read_messages(struct cw_Conn*conn);

extern int conn_recvfrom_packet(struct cw_Conn*conn, uint8_t * buf, int len,
			 struct sockaddr_storage *from);

int conn_send_msg(struct cw_Conn* conn, uint8_t *rawmsg);
int cw_read_from(struct cw_Conn* conn, struct sockaddr_storage * from);

int conn_send_msg(struct cw_Conn *conn, uint8_t * rawmsg);

void conn_clear_upd(struct cw_Conn*conn, int merge);

#endif	/* __CONN_H */
