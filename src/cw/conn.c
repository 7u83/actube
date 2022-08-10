#include <string.h>
#include <stdint.h>


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


#include "dbg.h"
#include "msgset.h"
#include "mavltypes.h"





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
 * Put a message to a buffer
 * This functions assumes, that a message header is
 * alread initilaized in buffer 
 * Message alements are taken fom actiondef in #conn->action
 */
int cw_assemble_message(struct cw_Conn *conn, uint8_t * rawout)
{
	char details[1024];
	uint8_t *msgptr,*dst;
	int type;
	struct cw_MsgData * msg;
	struct mlistelem * elem;
	int len,l;

	cw_dbg(DBG_INFO, "Number of elements in ktv: %d",conn->local_cfg->count);
/*	cw_dbg_ktv_dump(conn->local_cfg,DBG_CFG_DMP,"Local CFG","LOCAL:","End Local CFG");*/

	/* rawout is already initialized, so we can get 
	 * msg type from buffer */
	msgptr = rawout + cw_get_hdr_msg_offset(rawout);
	type = cw_get_msg_type(msgptr);

	/* look for message data */
	msg = cw_msgset_get_msgdata(conn->msgset,type);
	if (msg == NULL){
		cw_log(LOG_ERR,"Error: Can't create message of type %d (%s) - no definition found.",
			type, cw_strmsg(type));
		return CAPWAP_RESULT_MSG_UNRECOGNIZED;
	}

	if (msg->preprocess){
		msg->preprocess(conn);
	}

	cw_dbg(DBG_MSG_ASSEMBLY,"*** Assembling message of type %d (%s) ***", 
			msg->type, msg->name);
	
	dst = msgptr+8;
	len =0;
	mlist_foreach(elem,msg->elements_list){
		struct cw_ElemData * data;
		struct cw_ElemHandler * handler;
		struct cw_ElemHandlerParams params;
		
		data =  mlistelem_dataptr(elem);
		handler = cw_msgset_get_elemhandler(conn->msgset,data->proto,data->vendor,data->id);
printf("Elem: %d %d %d %s\n", data->proto, data->vendor, data->id, handler->name);
		if (handler==NULL){
			cw_log(LOG_ERR,"Can't put message element %d %d %d, no handler defined.",
					data->proto,data->vendor,data->id);
			continue;
		}

cw_dbg(DBG_X,"Hello!");

		if (handler->put == NULL){
			if (data->mand){
				cw_log(LOG_ERR,"Error: Can't add mandatory message element %d - %s, no put method defined",
					handler->id, handler->name);
				
			}
			continue;
		}

	/*	params.conn=conn;*/
		params.local_cfg=conn->local_cfg;
		params.remote_cfg=conn->remote_cfg;
		params.default_cfg=conn->default_cfg;
                params.global_cfg=conn->global_cfg;
		params.msgset=conn->msgset;



		params.elemdata = data;
		params.msgdata=msg;
		params.debug_details=details;
		*details=0;

/*		if (strcmp(handler->key,"cisco/ap-led-flash-config")==0){
			printf("flash config\n");
			 cisco/ap-led-flash-config/flash-enable 
		}*/

cw_dbg(DBG_X,"Calling Handler put for %s",handler->name);

		l = handler->put(handler,&params,dst+len);

cw_dbg(DBG_X,"L = %d",l);

	/*	if(l>0)
			cw_dbg_elem(DBG_ELEM_OUT,conn,type,handler,dst+len,l);
	*	if (strlen(details)){
			cw_dbg(DBG_ELEM_DETAIL,"  %s",params.debug_details);
		}
	*/	len += l;
	}

	cw_set_msg_elems_len(msgptr, len);
	cw_dbg(DBG_MSG_ASSEMBLY,"*** Done assenmbling message of type %d (%s) ***", 
			msg->type, msg->name);
	if (type & 1) {
		/* It's a request, so we have to set seqnum */
		int s = conn_get_next_seqnum(conn);
		cw_set_msg_seqnum(msgptr,s);
	}


	{
	printf ("----------------------------------- redecode -----------------------------\n");
	uint8_t *elems_ptr;

	int offset = cw_get_hdr_msg_offset(rawout);

	uint8_t *msg_ptr = rawout + offset;
	int elems_len = cw_get_msg_elems_len(msg_ptr);
	elems_ptr = cw_get_msg_elems_ptr(msg_ptr);
	mavl_t * cfg = cw_ktv_create();

	struct cw_ElemHandlerParams params;

	params.remote_cfg=cfg;
	params.msgset=conn->msgset;
	params.msgdata=msg;


	cw_decode_elements( &params, elems_ptr,elems_len);

	printf ("----------------------------------- end redecode -----------------------------\n");

	}

	return CAPWAP_RESULT_SUCCESS;


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


