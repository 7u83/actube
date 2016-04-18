#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include "cw/capwap.h"
#include "cw/conn.h"
#include "cw/radioinfo.h"
#include "cw/log.h"
#include "cw/dtls.h"
#include "cw/sock.h"
#include "cw/cw_util.h"
#include "cw/capwap_items.h"


#include "wtp_conf.h"


#include "cw/timer.h"



#include "cw/capwap.h"
#include "cw/conn.h"
#include "wtp_interface.h"

#include "cw/dbg.h"
#include "cfg.h"

int update =0;

int handle_update_req(struct conn *conn, struct cw_action_in *a, uint8_t * data,
		      int len, struct sockaddr *from)
{
	if (a->msg_id == CW_MSG_CONFIGURATION_UPDATE_REQUEST){
		update =1;

	}

	return 0;

	MAVLITER_DEFINE(it, conn->incomming);

	mavliter_foreach(&it) {
		mbag_item_t *item = mavliter_get(&it);


	}
cw_dbg(DBG_X,"Saving configuration ...");	
		cfg_to_json();

	return 0;

}





static void update_radio(struct conn * conn, int rid, mbag_t radio_upd, mbag_t radio,mbag_t iresult )
{

	printf("Updating radio with rid %d\n",rid);
}

static void update_radios(struct conn * conn, mbag_t result)
{
	MAVLITER_DEFINE (it,conn->radios_upd);
	mavliter_foreach(&it){
		struct mbag_item * item = mavliter_get(&it);
		int rid = item->iid;
		mbag_t radio = mbag_i_get_mbag(conn->radios,rid,NULL);
		if (!radio){
			cw_dbg(DBG_ELEM_ERR,"Can't find radio with id %d",rid);
			continue;
		}

		mbag_t iresult = mbag_create();
		mbag_i_set_mbag(result,rid,iresult);		
		update_radio(conn,rid,item->data,radio,iresult);
	}

}


static void do_update(struct conn * conn)
{
	if (!update)
		return;
	update=0;

	mbag_t result = mbag_create();
	update_radios(conn,result);
	cw_dbg(DBG_INFO, "Saving configuration ...");
	cfg_to_json();
	/* Change State ... */
	int rc = cw_send_request(conn,CW_MSG_CHANGE_STATE_EVENT_REQUEST);
	if ( !cw_rcok(rc) ) {
		cw_strresult(rc);
		return ;
	}



}




int run()
{


	struct conn *conn = get_conn();
	conn->capwap_state = CW_STATE_RUN;

	conn->msg_end=handle_update_req;

	do {

		int echo_interval = mbag_get_word(conn->config,CW_ITEM_CAPWAP_TIMERS,CAPWAP_TIMERS)&0xff;
		time_t timer = cw_timer_start(echo_interval);
		int rc;

		


		while (!cw_timer_timeout(timer) && conn->capwap_state == CW_STATE_RUN) {
			rc = cw_read_messages(conn);
			if (rc < 0 && errno == EAGAIN) {
				continue;
			}

			if ( !cw_rcok(rc))
				break;


			cw_dbg(DBG_X,"We hav a message processed");

			do_update(conn);


		}
		if (rc<0 && errno == EAGAIN){
			rc = cw_send_request(conn,CW_MSG_ECHO_REQUEST);
		
			if (!cw_rcok(rc)) {
				cw_log(LOG_ERR,"Error in run state: %d %s",rc,cw_strrc(rc));
				break;
			}
			continue;
		}

		if (!cw_rcok(rc)) {
			cw_log(LOG_ERR,"Error in run state: %d %s",rc,cw_strrc(rc));
			break;
		}
		



	} while (conn->capwap_state == CW_STATE_RUN);



//      int rc = cw_send_request(conn,CW_MSG_CHANGE_STATE_EVENT_REQUEST);

//      if ( !cw_rcok(rc) ) {
//              cw_strresult(rc);
//      }

	return 0;
}









/*
static int echo_interval_timer;

struct cwrmsg * get_response(struct conn * conn, int type,int seqnum)
{
	struct cwrmsg * cwrmsg; 
	int i;
	for(i=0; i<conf_retransmit_interval; i++){
		cwrmsg = conn_get_message(conn);
		if ( cwrmsg==0){
			//printf("null message \n");
			continue;
		}

		if (cwrmsg->type==type && cwrmsg->seqnum==seqnum)
			return cwrmsg;
		printf("another message was detected %i %i\n",cwrmsg->type,cwrmsg->seqnum);
	}		
	return 0;
	
}
*/


/*
struct cwrmsg * send_request(struct conn * conn,struct cwmsg *cwmsg)
{
	int i;
	for (i=0; i<conf_max_retransmit; i++){

#ifdef WITH_CW_LOG_DEBUG
		if (i>0){
//			cw_log_debug1("Retransmitting request, type=%i,seqnum=%i",cwmsg->type,cwmsg->seqnum);
		}
#endif

		int rc = conn_send_cwmsg(conn,cwmsg);
		if (rc<0){
		//	cw_log_debug1("Error sending request, type=%i, seqnum %i, %s",cwmsg->type,cwmsg->seqnum,strerror(errno));
			return 0;
		}
		struct cwrmsg * r = get_response(conn,cwmsg->type+1,cwmsg->seqnum);
		if (r)
			return r;

	}
	return 0;
}

*/

//extern struct conn * get_conn();

/*
int run(struct conn * conn)
{

	conn = get_conn();
	printf("Running with conn %p\n");

	struct radioinfo radioinfo;
	memset(&radioinfo,0,sizeof(radioinfo));


	struct cwrmsg * cwrmsg;

	echo_interval_timer=time(NULL);
	while (1){	
		if (time(NULL)-echo_interval_timer >= conf_echo_interval)
		{
	//		struct cwmsg cwmsg;
	//		uint8_t buffer[CWMSG_MAX_SIZE];

		//	cwsend_echo_request(conn,&radioinfo);

//			cw_log_debug1("Sending echo request");
			struct cwmsg *cwmsg=&conn->req_msg;
			uint8_t * buffer = conn->req_buffer;


	struct wtpinfo * wtpinfo = get_wtpinfo();
	struct radioinfo *rip = &(wtpinfo->radioinfo[0]);

			cwmsg_init_echo_request(cwmsg,buffer,conn,rip);

printf("Echo ->>>>>>>>>>>>>>>>>>>>> Seqnum %d\n",conn->req_msg.seqnum);


printf("Conn target is %s",sock_addr2str(&conn->addr));
printf("Calling conn send req\n");
printf("conn max retrans: %d\n",conn->max_retransmit);
			struct cwrmsg * rc = conn_send_request(conn);
printf("Back from conn send req\n");

//			printf("conn->seqnum %i\n",conn->seqnum);
//			struct cwrmsg * rc = get_response(conn,CWMSG_ECHO_RESPONSE,conn->seqnum);		
			if (rc==0){

printf("Error !\n");

//				dtls_shutdown(conn);	
		//		cw_log_debug1("Connection lost, no echo response");
//				return 0;
			}
			echo_interval_timer=time(NULL);
		}

		time_t rt = cw_timer_start(5);	
		cwrmsg = conn_wait_for_request(conn,0,rt);
	struct wtpinfo * wtpinfo = get_wtpinfo();
	struct radioinfo *rip = &(wtpinfo->radioinfo[0]);

		if(cwrmsg){
			cw_readmsg_configuration_update_request(cwrmsg->msgelems,cwrmsg->msgelems_len);
			cw_send_configuration_update_response(conn,cwrmsg->seqnum,rip);
		}

		sleep(1);

	}
	exit(0);
}


*/
