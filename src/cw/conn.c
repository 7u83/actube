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

	//cw_dbg(DBG_INFO, "Number of elements in ktv: %d",conn->local_cfg->count);
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
//		cw_dbg(DBG_X,"Elem: %d %d %d %s\n", data->proto, data->vendor, data->id, handler->name);
		if (handler==NULL){
			cw_log(LOG_ERR,"Can't put message element %d %d %d, no handler defined.",
					data->proto,data->vendor,data->id);
			continue;
		}


		if (handler->put == NULL){
			if (data->mand){
				cw_log(LOG_ERR,"Error: Can't add mandatory message element %d - %s, no put method defined",
					handler->id, handler->name);
				
			}
			continue;
		}

		params.conn=conn;
		params.cfg=conn->remote_cfg;
		params.default_cfg=conn->default_cfg;

		params.msgset=conn->msgset;



		params.elemdata = data;
		params.msgdata=msg;
		params.debug_details=details;
		*details=0;

/*		if (strcmp(handler->key,"cisco/ap-led-flash-config")==0){
			printf("flash config\n");
			 cisco/ap-led-flash-config/flash-enable 
		}*/


		l = handler->put(handler,&params,dst+len);


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
	mavl_t * cfg = cw_cfg_create();

	struct cw_ElemHandlerParams params;

	params.cfg=cfg;
	params.default_cfg=NULL;
	params.msgset=conn->msgset;
	params.msgdata=msg;


	cw_decode_elements( &params, elems_ptr,elems_len);

	printf ("----------------------------------- end redecode -----------------------------\n");

	}

	return CAPWAP_RESULT_SUCCESS;


}



struct msg_callback{
	int type; /**< message type */
	cw_MsgCallbackFun fun;
};

int msg_callback_cmp(const void *v1,const void *v2)
{
	struct msg_callback *t1,*t2;
	t1=(struct msg_callback*)v1;
	t2=(struct msg_callback*)v2;
	return t1->type - t2->type;
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
	conn->strict_capwap=1;

	conn->process_packet=conn_process_packet;
	conn->process_message=process_message;

	conn->msg_callbacks = mavl_create(msg_callback_cmp,NULL,sizeof(struct msg_callback));
}

int cw_conn_set_msg_cb(struct cw_Conn *conn, int type, cw_MsgCallbackFun fun)
{
	struct msg_callback cb;
	int exists;

	cb.type = type;
	cb.fun = fun;
	mavl_insert(conn->msg_callbacks,&cb,&exists);
}

cw_MsgCallbackFun cw_conn_get_msg_cb(struct cw_Conn *conn, int type)
{
	struct msg_callback cb,*result;
	cb.type=type;
	result = mavl_get(conn->msg_callbacks,&cb);
	if (result == NULL)
		return NULL;
	return result->fun;
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

	conn->dtls_mtu = 600;




	return conn;
}


