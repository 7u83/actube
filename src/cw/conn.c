#include <string.h>


#include "conn.h"
#include "capwap.h"
#include "cw.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "log.h"
#include "capwap.h"
#include "conn.h"
#include "sock.h"

#include "msgset.h"
#include "cw.h"




static int write_header(struct cw_ElemHandler * handler, uint8_t * dst, int len)
{
	if (handler->vendor)
		return len + cw_put_elem_vendor_hdr(dst, handler->vendor, handler->id, len);

	return  len + cw_put_elem_hdr(dst, handler->id, len);
}

static int header_len(struct cw_ElemHandler * handler)
{
	return handler->vendor ? 10 : 4;
}


/**
 * Basic initialization of a conn object 
 * @param conn conn object to initialize
 */ 
void cw_conn_init(struct cw_Conn * conn)
{
	memset(conn,0,sizeof(struct cw_Conn ));
	conn->retransmit_interval=CAPWAP_RETRANSMIT_INTERVAL;
	conn->max_retransmit=CAPWAP_MAX_RETRANSMIT;
	conn->wait_dtls=CAPWAP_WAIT_DTLS;
	conn->wait_join=CAPWAP_WAIT_JOIN;
	conn->mtu_discovery=1;
//	conn->capwap_mode = 0;
	conn->strict_capwap=1;



	conn->process_packet=conn_process_packet;
	conn->process_message=process_message;

	conn->write_header = write_header;
	conn->header_len = header_len;


}

/**
 * Create a conn object
 * @param sock a socket
 * @param addr the address associated
 * @param qsize size of packet queue
 * @return A pointer to the created object
 * This function creates a conn obnject with queueing functionality
 * for asynchronous operation. 
 * To create a conn object without queue functionallity use  #conn_create_noq.
 */
struct cw_Conn * cw_conn_create(int sock, struct sockaddr * addr, int qsize)
{
	struct cw_Conn * conn; 
	conn = malloc(sizeof (struct cw_Conn ));
	if (!conn)
		return NULL;

	cw_conn_init(conn);

	conn->sock=sock;

	if (addr)
		sock_copyaddr(&conn->addr,addr);


	conn->fragman = fragman_create();
	if (conn->fragman==NULL){
		conn_destroy(conn);
		return NULL;
	}

	conn->qsize=qsize;
	if (qsize != 0){
		if (!(conn->q=malloc( sizeof(uint8_t *) * qsize))){
			conn_destroy(conn);
			return NULL;
		}
		conn->qrpos=-1;
		if (sem_init(&conn->q_sem,0,0)!=0){
			cw_log(LOG_ERR,"Fatal- Can't init semaphore for conn object: %s",strerror(errno));
			conn_destroy(conn);
			return NULL;
		};
		conn->recv_packet=conn_q_recv_packet;
		conn->recv_packet_peek=conn_q_recv_packet_peek;
	}
	else{
		conn->recv_packet = conn_recv_packet;
		conn->recv_packet_peek = conn_recv_packet_peek;
	}

	conn->send_packet = conn_send_packet;
/*	conn->send_data_packet = conn_send_data_packet;*/

	conn->last_seqnum_received=-1;
	conn->mtu=600;


	conn->cur_packet=0;
	conn->recv_timeout=1;

	conn->seqnum=-1;
	conn->write = conn->send_packet;
	conn->read = conn->recv_packet;

/*	conn->write_data = conn->send_data_packet; */

	conn->dtls_mtu = 600;

	return conn;
}


