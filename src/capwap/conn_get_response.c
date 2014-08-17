
#include "capwap.h"
#include "cw_log.h"
#include "sock.h"
#include "cw_util.h"

#include <stdio.h>

struct cwrmsg * conn_get_response(struct conn * conn)
{

	printf("Starting timer 120\n");
	time_t timer = cw_timer_start(160);

	struct cwmsg * cwmsg = &conn->req_msg;

	int type = cwmsg->type+1;


	printf ("Awaiting response message for type=%d\n",type);



	struct cwrmsg * cwrmsg;

	time_t rpt_timer = cw_timer_start(5);
        do {
                cwrmsg = conn_get_message(conn);


                if (cwrmsg){
		        cw_dbg(DBG_CW_MSG,"Received message from %s, type=%d - %s"
               			 ,sock_addr2str(&conn->addr),cwrmsg->type,cw_msgtostr(cwrmsg->type));
                        if (cwrmsg->type == type){
				printf("Jea!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11\n");
                        	return cwrmsg;        
                        }
			
                }

		if (cw_timer_timeout(rpt_timer)){
			rpt_timer = cw_timer_start(5);
			printf("Retransmitting request\n");
			conn_send_cwmsg(conn,&conn->req_msg);
		}



                if (conn->dtls_error)
                        return 0;

        }while(!cw_timer_timeout(timer));

	printf("Timeout!!!!!\n");

	return 0;

}
