#include <errno.h>

#include "conn.h"
#include "capwap.h"
#include "cw_log.h"
#include "sock.h"

int cw_send_request(struct conn *conn,int msg_id)
{
	cw_init_request(conn, msg_id);
	if ( cw_put_msg(conn, conn->req_buffer) == -1 )
		return 0;
	conn_send_msg(conn, conn->req_buffer);


	
	
	int i;
	int rc=-1;
	for (i=0; i<conn->max_retransmit && rc<0; i++){
		time_t timer = cw_timer_start(conn->retransmit_interval);
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

	if ( rc <0 ) {
		cw_log(LOG_ERR,"Error reading from %s:%s",sock_addr2str(&conn->addr),strerror(errno));
	}

	return rc;
}
