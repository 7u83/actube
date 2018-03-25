#ifndef __CONN_LINK_H
#define __CONN_LINK_H


#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "fragman.h"

struct netconn;

struct netconn{

	int sock;
	struct sockaddr_storage addr;



	/** The framgent manager used on this connection */
	frag_t *fragman;


	/** Current Fragment ID */
	int fragid;

	int (*recv_packet) (struct netconn *, uint8_t *, int);
	int (*recv_packet_peek) (struct netconn *, uint8_t *, int);
	int (*send_packet) (struct netconn *, const uint8_t *, int);

	int (*readfrom) (struct netconn *, uint8_t *, int, struct sockaddr_storage *);
	int (*read) (struct netconn *, uint8_t *, int);
	int (*write) (struct netconn *, const uint8_t *, int);


	/* optional packet queue */
	uint8_t **q;
	int qsize;
	int qrpos;
	int qwpos;
	sem_t q_sem;
	uint8_t *cur_packet;
	int cur_packet_len;
	int cur_packet_pos;

	int (*process_packet)(struct netconn *nc, uint8_t * packet, int len,struct sockaddr *from);
	int (*process_message)(struct netconn *nc, uint8_t * rawmsg, int rawlen,
			   struct sockaddr *from);

	void * data;

};

extern struct netconn * netconn_create(int sock, struct sockaddr * addr, int qsize);
extern void netconn_destroy(struct netconn *nc);
extern void netconn_q_add_packet(struct netconn * nc,uint8_t *packet,int len);
extern int netconn_process_packet(struct netconn *nc, uint8_t * packet, int len,
			struct sockaddr *from);
extern int netconn_read_messages(struct netconn *nc);
extern int netconn_send_capwap_msg(struct netconn * nc, uint8_t *rawmsg, int msglen);



#endif
