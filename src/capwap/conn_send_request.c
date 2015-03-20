/**
 *@file
 *@brief conn_send_request 
 */

#include "capwap.h"
#include "cw_util.h"
#include "cw_log.h"

/**
 * Send a request message and wait for response
 * @param conn the connection
 * 
 */ 
struct cwrmsg * conn_send_request(struct conn * conn)
{
	int i;

	struct cwrmsg * cwrmsg;
	struct cwmsg * cwmsg = &conn->req_msg;


        for (i=0; i<conn->max_retransmit; i++) {

                time_t r_timer = cw_timer_start(conn->retransmit_interval);
		if (i!=0)
	                cw_dbg(DBG_CW_MSG_ERR,"Retransmitting message, type=%d,seq=%d",cwmsg->type,cwmsg->seqnum);

		conn_send_cwmsg(conn,&conn->req_msg);
		cwrmsg = conn_wait_for_message(conn,r_timer);
		if (cwrmsg){
			if (cwrmsg->type == conn->req_msg.type+1){
				return cwrmsg;        
			}
                                
		}
        }
        cw_dbg(DBG_CW_MSG_ERR,"Max retransmit's reached, message type=%d,seq=%d",cwmsg->type,cwmsg->seqnum);
        return 0;
}

