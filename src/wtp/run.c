#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include "capwap/capwap.h"
#include "capwap/conn.h"
#include "capwap/radioinfo.h"
#include "capwap/log.h"
#include "capwap/dtls.h"
#include "capwap/sock.h"
#include "capwap/cw_util.h"


#include "wtp_conf.h"

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
