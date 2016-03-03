
#include "capwap.h"
#include "cw_log.h"
#include "sock.h"
#include "cw_util.h"


struct cwrmsg * conn_get_response(struct conn * conn)
{


	struct cwmsg * cwmsg = &conn->req_msg;

	int type = cwmsg->type+1;

	struct cwrmsg * cwrmsg;

	int i;
        for (i=0; i<conn->max_retransmit; i++) {

		time_t r_timer = cw_timer_start(conn->retransmit_interval);

		while(!cw_timer_timeout(r_timer)){
	                cwrmsg = conn_get_message(conn);
	                if (cwrmsg){
			        cw_dbg(DBG_MSG,"Received message from %s, type=%d - %s"
	               			 ,sock_addr2str(&conn->addr),cwrmsg->type,cw_msgtostr(cwrmsg->type));
	                        if (cwrmsg->type == type){
	                        	return cwrmsg;        
               			 }
				
        	        }

			if (conn->dtls_error)
	                        return 0;
		}
		cw_dbg(DBG_MSG_ERR,"Retransmitting message, type=%d,seq=%d",cwmsg->type,cwmsg->seqnum);
		conn_send_cwmsg(conn,&conn->req_msg);

        }
	cw_dbg(DBG_MSG_ERR,"Max retransmit's reached, message type=%d,seq=%d",cwmsg->type,cwmsg->seqnum);
	return 0;

}
