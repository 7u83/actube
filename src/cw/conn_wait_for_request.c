
#include "conn.h"
#include "cw_log.h"
#include "sock.h"
#include "cw_util.h"

struct rh_param{
        struct conn * conn;
        int *msglist;

};


static int conn_rh(void *param)
{
        struct rh_param * p = (struct rh_param*)param;
        int i;
        int *msglist=p->msglist;


printf("Param %p\n",param);


        for (i=0; msglist[i]!=-1; i++){
                if (msglist[i] == p->conn->cwrmsg.type )
                        return 0;

        }
        /* unexpected response here */
        cw_log(LOG_ERR,"Unexpected message from %s",sock_addr2str(&p->conn->addr));
        cwsend_unknown_response(p->conn,p->conn->cwrmsg.seqnum,p->conn->cwrmsg.type);
        return 1;
}


struct cwrmsg * conn_wait_for_request(struct conn * conn, int *msglist, time_t timer)
{
        int (*request_handler_save) (void*);
        void * request_handler_param_save;

        struct rh_param rh_param;


        if (msglist){
                request_handler_save=conn->request_handler;
                request_handler_param_save=conn->request_handler_param;
                rh_param.conn=conn;
                rh_param.msglist=msglist;
                conn->request_handler=conn_rh;
                conn->request_handler_param=&rh_param;
        }


        struct cwrmsg * cwrmsg;
        while (!cw_timer_timeout(timer)){
                cwrmsg = conn_wait_for_message(conn,timer);
                if (!cwrmsg){
                        if (!conn_is_error(conn))
                                continue;
                        break;
                }

                /* response message?  no action*/
                if (! (cwrmsg->type & 1) )
                        continue;

                /* it's a request message */
                break;
        }

        if (msglist){
                conn->request_handler=request_handler_save;
                conn->request_handler_param=request_handler_param_save;
        }

        return cwrmsg;
}

