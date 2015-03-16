#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <arpa/inet.h>

#include "wtplist.h"

#include "capwap.h"
#include "sock.h"
#include "socklist.h"

#include "conn.h"
#include "wtpman.h"
#include "conf.h"
#include "cw_log.h"

#include "dtls.h"



#include "conf.h"

#include "lwmsg.h"
#include "lwapp.h"


#include <errno.h>

#include "capwap.h"
#include "capwap_cisco.h"

#include "cw_util.h"


/* macro to convert our client ip to a string */
#define CLIENT_IP (sock_addrtostr((struct sockaddr*)&wtpman->conn->addr, (char[64]){0},64))

/*
int conn_handle_echo_request(void * d)
{
	struct conn * conn = (struct conn *)d;
	struct cwrmsg * cwrmsg = &conn->cwrmsg;
	cwsend_echo_response(conn,cwrmsg->seqnum,0);
	return 0;
}
*/


void conn_handle_change_state_event_request(struct conn * conn)
{
}


static struct cwrmsg * xconn_wait_for_message(struct conn * conn, time_t timer)
{
	struct cwrmsg * cwrmsg;


	while (!cw_timer_timeout(timer)){
		cwrmsg = conn_get_message(conn);

		if (!cwrmsg){
			if (!conn_is_error(conn))
				continue;

			return 0;
		}

		if (cwrmsg->type & 1){
			if (conn->request_handler){
				if (conn->request_handler(conn->request_handler_param))
					continue;
			}

			
		}

		return cwrmsg;

	}

	return 0;
}


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

static struct cwrmsg * conn_wait_for_request(struct conn * conn, int *msglist, time_t timer)
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





struct cwrmsg * xconn_send_request(struct conn * conn)
{
	int i;

	struct cwrmsg * cwrmsg;
	struct cwmsg * cwmsg = &conn->req_msg;
	
printf("Retrans interval = %d\n",conn->retransmit_interval);
printf("Current Seqnum = %d\n",conn->seqnum);


        for (i=0; i<conn->max_retransmit; i++) {

                time_t r_timer = cw_timer_start(conn->retransmit_interval);
		if (i!=0)
	                cw_dbg(DBG_CW_MSG_ERR,"Retransmitting message, type=%d,seq=%d",cwmsg->type,cwmsg->seqnum);

		conn_send_cwmsg(conn,&conn->req_msg);
		cwrmsg = conn_wait_for_message(conn,r_timer);
		if (cwrmsg){
			if (cwrmsg->type == conn->req_msg.type+1){
				printf("YeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaA!!!!!\n");
				return cwrmsg;        
			}

//                	cw_dbg(DBG_CW_MSG_ERR,"Wrong message blablub, type=%d,seq=%d",cwmsg->type,cwmsg->seqnum);
			
                                
		}

        }
        cw_dbg(DBG_CW_MSG_ERR,"Max retransmit's reached, message type=%d,seq=%d",cwmsg->type,cwmsg->seqnum);
        return 0;

}

int wtpman_handle_request(void *p)
{
	struct wtpman * wtpman = (struct wtpman *)p;
	struct conn * conn = wtpman->conn;
	struct cwrmsg * cwrmsg = &conn->cwrmsg;
	switch(conn->cwrmsg.type){
		case CWMSG_ECHO_REQUEST:
			cw_handle_echo_request(conn);
			break;
		case CWMSG_CHANGE_STATE_EVENT_REQUEST:
			cwread_change_state_event_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);
			cwsend_change_state_event_response(wtpman->conn,cwrmsg->seqnum,wtpman->wtpinfo.radioinfo);
			break;
		default:
			printf("Unhandeleed  message %d!!!!!!!!!!!!\n",cwrmsg->type);
			cwsend_unknown_response(conn,cwrmsg->seqnum,cwrmsg->type);
	}
	return 1;
}


void send_image_file(struct conn * conn,const char * filename)
{
	FILE * infile;
	infile = fopen(filename,"rb");
	if (!infile){
		cw_log(LOG_ERR,"Can't open image file %s:%s",filename,strerror(errno));
		return;
	}


	



	cw_log(LOG_INFO,"Sending image file %s to %s",filename,sock_addr2str(&conn->addr));

	struct cwrmsg * cwrmsg;
	uint8_t  buffer[1024];
	struct image_data data;
	data.data = buffer;


	conn->request_handler = cw_handle_echo_request;
	conn->request_handler_param = conn;

int bl=0;

//fseek(infile,2703*1024,SEEK_SET);
	do{

		data.len = fread(buffer,1,sizeof(buffer),infile);
//data.len=0;

/*
memset(buffer,0xff,1024);
memset(buffer+100,7,77);
memset(buffer+300,251,77);
buffer[1000]=99;
buffer[777]=99;
buffer[778]=199;
buffer[178]=199;
buffer[179]=199;
buffer[279]=199;
buffer[319]=219;

*/
int ai;
for (ai=0; ai<1024; ai++){
	printf("%02X ",buffer[ai]);

}



		if (feof(infile))
			data.type=2;
		else	
			data.type=1;

		if (data.len==0){
			printf("*******************************************len0************************************\n");
		}

		printf("Send img data request, block=%d, len=%d, ch=%d\n",bl,data.len,lw_checksum(data.data,data.len));

//exit(0);

		bl++;

		conn_prepare_image_data_request(conn,&data,0);
		cwrmsg = conn_send_request(conn);


		if (cwrmsg){
			printf("Got img data response\n");
		}
		else{
			printf("Respnse Timeout\n");
			exit(0);
		}


	}while(!feof(infile));


	exit(0);


}


//ACIPLIST * get_aciplist();
struct ac_info * get_acinfo();

static void wtpman_remove(struct wtpman * wtpman)
{
	wtplist_lock();
	wtplist_remove(wtpman);
	wtplist_unlock();
	wtpman_destroy(wtpman);
}


/*
 * Waits for a capwap message until message is received or timeout occurs
 * Returns NULL if timeout
 * Returns pointer to cwrms if message is received
 * Returns EOF in case of any other error
 */
static struct cwrmsg * wtpman_wait_for_message(struct wtpman * wtpman, time_t timer)
{
	struct cwrmsg * cwrmsg;
	do {
		cwrmsg = conn_get_message(wtpman->conn);
		if (!cwrmsg && wtpman->conn->dtls_error)
			return (struct cwrmsg*)EOF;
		if (!cwrmsg && cw_timer_timeout(timer)) 
			return NULL;

	}while(!cwrmsg);

	cw_dbg(DBG_CW_MSG,"Received message from %s, type=%d - %s"
		,CLIENT_IP,cwrmsg->type,cw_msgtostr(cwrmsg->type));

	return cwrmsg;
}



static void wtpman_run_discovery(void *arg)
{


	struct wtpman * wtpman = (struct wtpman *)arg;
	struct cwrmsg * cwrmsg;


	time_t timer = cw_timer_start(10);
	cwrmsg = wtpman_wait_for_message(wtpman, timer);

	if ( !cwrmsg  )
	{
		cw_dbg(DBG_CW_MSG_ERR,"No complete message from %s received after %d seconds",CLIENT_IP,10);
		wtpman_remove(wtpman);
		return;
	}

	
	if (cwrmsg->type!=CWMSG_DISCOVERY_REQUEST){
		cw_dbg(DBG_CW_MSG_ERR,"Invalid message in discovery state from %s, type=%s - %s ",
			CLIENT_IP,cwrmsg->type,cw_msgtostr(cwrmsg->type));
		wtpman_remove(wtpman);
		return;
	}

	cwread_discovery_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);



	printf("RMAC-LEN:%d\n",cwrmsg->rmac[0]);

printf("HW: %s\n",sock_hwaddr2str(bstr_data(cwrmsg->rmac),bstr_len(cwrmsg->rmac)));



	conn_detect_capwap(wtpman->conn,&wtpman->wtpinfo);

	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
	cw_dbg(DBG_CW_INFO,"Discovery request gave us the follwing WTP Info:\n%s",wtpinfostr);



	struct radioinfo radioinfo;
	radioinfo.rid = cwrmsg->rid;
	radioinfo.rmac = 0; //&cwrmsg->rmac;
	
//	memcpy(radioinfo.rmac, cwrmsg->rmac,8);
//	radioinfo.rmac[0]=0;

	struct ac_info * acinfo = get_acinfo();



/*	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
	cw_dbg(DBG_CW_INFO,"Discovery request gave us the follwing WTP Info:\n%s",wtpinfostr);
*/

	cwsend_discovery_response(wtpman->conn,cwrmsg->seqnum,&radioinfo,acinfo,&wtpman->wtpinfo);			


	wtpman_remove(wtpman);
}


static void wtpman_run_run(void *arg)
{

	
	struct wtpman * wtpman = (struct wtpman *)arg;
	struct conn * conn = wtpman->conn;

	struct cwrmsg * cwrmsg;


	conn->request_handler=wtpman_handle_request;
	conn->request_handler_param=wtpman;

	
	int i;
	for (i=0; i<10; i++){
		time_t t = cw_timer_start(1);
		printf("Wait...\n");
		conn_wait_for_message(conn,t);
	}

	printf("Update now?\n");



	conn_prepare_request(conn,CWMSG_CONFIGURATION_UPDATE_REQUEST);
	cwmsg_addelem(&conn->req_msg,CWMSGELEM_WTP_NAME,(uint8_t*)"Tube7u83",strlen("Tube7u83")+1);
	cwmsg_addelem(&conn->req_msg,CWMSGELEM_LOCATION_DATA,(uint8_t*)"Berlin",strlen("Berlin")+1);

	cwmsg_addelem_vendor_specific_payload(&conn->req_msg,CW_VENDOR_ID_CISCO,CWVENDOR_CISCO_RAD_NAME,(uint8_t*)"NudelSuppe",strlen("NudelSuppe"));

	cwrmsg = conn_send_request(conn);


	for (i=0; i<10; i++){
		time_t t = cw_timer_start(1);
		printf("Wait...\n");
		conn_wait_for_message(conn,t);
	}

/*	conn_prepare_request(conn,CWMSG_RESET_REQUEST);
	cwmsg_addelem_image_identifier(&conn->req_msg,CW_VENDOR_ID_CISCO,"image00",strlen("image00"));
	cwrmsg = conn_send_request(conn);
*/	

	printf("Set name?\n");
	exit(0);	


}



static int wtpman_establish_dtls(void *arg)
{
	struct wtpman * wtpman = (struct wtpman *)arg;
	
	/* setup cipher */
	wtpman->conn->dtls_cipher=conf_sslcipher;

	/* setup DTSL certificates */
	int dtls_ok=0;
	if (conf_sslkeyfilename && conf_sslcertfilename){


		wtpman->conn->dtls_key_file = conf_sslkeyfilename;
		wtpman->conn->dtls_cert_file = conf_sslcertfilename;
		wtpman->conn->dtls_key_pass = conf_sslkeypass;
		wtpman->conn->dtls_verify_peer = conf_dtls_verify_peer;
		cw_dbg(DBG_DTLS,"DTLS - Using key file %s",wtpman->conn->dtls_key_file);
		cw_dbg(DBG_DTLS,"DTLS - Using cert file %s",wtpman->conn->dtls_cert_file);
		dtls_ok=1;
	}

	/* setup DTLS psk */
	if (conf_dtls_psk){
		wtpman->conn->dtls_psk=conf_dtls_psk;
		wtpman->conn->dtls_psk_len=strlen(conf_dtls_psk);
		dtls_ok=1;
	}

	if (!dtls_ok){
		cw_log(LOG_ERR,"Can't establish DTLS session, neither psk nor certs set in config file.");
		return 0;
	}

	/* try to accept the connection */
	if ( !dtls_accept(wtpman->conn) ){
		cw_dbg(DBG_DTLS,"Error establishing DTLS session with %s",CLIENT_IP);
		return 0;
	}
	
	cw_dbg(DBG_DTLS,"DTLS session established with %s, cipher=%s", CLIENT_IP,dtls_get_cipher(wtpman->conn));
	/* DTLS handshake done */


	int cert_len;
	struct dtls_ssl_cert cert;

	FILE *f;
	f = fopen ("pcert.der","wb");

	printf("Calling peers cert\n");
	cert = dtls_get_peers_cert(wtpman->conn,0);
	printf("Back from peers cert\n");
	
	printf("Have Peers Cert: %p, %d\n",cert.data,cert.size);
	fwrite(cert.data,1,cert.size,f);
	


	return 1;
}

static int wtpman_join(void *arg,time_t timer)
{
	struct wtpman * wtpman = (struct wtpman *)arg;

	/* timer = cw_timer_start(wtpman->conn->wait_join); */

	int join_msgs[] = { CWMSG_JOIN_REQUEST, -1 };
	struct cwrmsg * cwrmsg;	
	cwrmsg =  conn_wait_for_request(wtpman->conn, join_msgs, timer);

	if (!cwrmsg){
		if (conn_is_error(wtpman->conn)){
			cw_dbg(DBG_CW_MSG_ERR,"DTLS connection closed while waiting for join request from %s.",CLIENT_IP);
			return 0;
		}
			
		cw_dbg(DBG_CW_MSG_ERR,"No join request from %s after %d seconds, WTP died.",
		sock_addr2str(&wtpman->conn->addr),wtpman->conn->wait_dtls);
		return 0;
	
	}


	process_join_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);
	conn_detect_capwap(wtpman->conn,&wtpman->wtpinfo);

	{
	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
	cw_dbg(DBG_CW_INFO,"Join request gave us the follwing WTP Info:\n%s",wtpinfostr);
	}


	struct radioinfo radioinfo;
	radioinfo.rid = cwrmsg->rid;
//	memcpy (radioinfo.rmac, cwrmsg->rmac,8);
	struct ac_info * acinfo = get_acinfo();


	int result_code = 0;
	cw_dbg(DBG_CW_MSG,"Sending join response to %s",CLIENT_IP);
printf("SLeep befor join resp\n");
printf("Slept befor join resp\n");

	cwsend_join_response(wtpman->conn,cwrmsg->seqnum,result_code,&radioinfo,acinfo,&wtpman->wtpinfo);
	cw_log(LOG_INFO,"WTP joined, Name = %s, Location = %s, IP = %s",
		wtpman->wtpinfo.name,wtpman->wtpinfo.location,
		sock_addr2str(&wtpman->conn->addr));


	return 1;
	
}

static void wtpman_run(void *arg)
{
	struct wtpman * wtpman = (struct wtpman *)arg;
	struct cwrmsg * cwrmsg; // = conn_get_message(wtpman->conn);

	wtpman->conn->seqnum=0;

	/* reject connections to our multi- or broadcast sockets */
	if (socklist[wtpman->socklistindex].type != SOCKLIST_UNICAST_SOCKET){
		cw_dbg(DBG_DTLS,"Dropping connection from %s to non-unicast socket.", CLIENT_IP);
		wtpman_remove(wtpman);
		return;
	}


	time_t timer = cw_timer_start(wtpman->conn->wait_dtls);

	/* establish dtls session*/
	if (!wtpman_establish_dtls(wtpman)){
		wtpman_remove(wtpman);
		return;
	}


	/* dtls is established, goto join state */
	if (!wtpman_join(wtpman,timer)){
		wtpman_remove(wtpman);
		return;
	}




	/* here the WTP has joined, now image update or change state event */

	int cfg_status_msgs[] = { CWMSG_IMAGE_DATA_REQUEST, CWMSG_CONFIGURATION_STATUS_REQUEST, -1 };
	cwrmsg =  conn_wait_for_request(wtpman->conn, cfg_status_msgs, timer);

	if (!cwrmsg){
		cw_dbg(DBG_CW_MSG_ERR,"No config uration status request from %s after %d seconds, WTP died.",
			sock_addr2str(&wtpman->conn->addr),wtpman->conn->wait_join);
		wtpman_remove(wtpman);	
		return;
	}
printf("Have Masseg %d\n",cwrmsg->type);
	cwread_configuration_status_request(&wtpman->wtpinfo,cwrmsg->msgelems, cwrmsg->msgelems_len);
	int result_code=0;
	struct ac_info *acinfo = get_acinfo();
	struct radioinfo rinf;
	cwsend_conf_status_response(wtpman->conn,cwrmsg->seqnum,result_code,&rinf,acinfo,&wtpman->wtpinfo);
printf("Send the respi but sleep\n");

	
printf("Next thoing\n");
	int change_status_msgs[] = { CWMSG_IMAGE_DATA_REQUEST,CWMSG_CHANGE_STATE_EVENT_REQUEST, -1 };
	cwrmsg =  conn_wait_for_request(wtpman->conn, change_status_msgs, timer);
printf("Done\n");

	if (!cwrmsg){
		wtpman_remove(wtpman);	
		return;
	}



	switch (cwrmsg->type){
		case CWMSG_CHANGE_STATE_EVENT_REQUEST:
			{
			printf("Change state event\n!");
			struct radioinfo ri;
			cwsend_change_state_event_response(wtpman->conn,cwrmsg->seqnum,&ri);
			}
			break;
		case CWMSG_IMAGE_DATA_REQUEST:
			printf("Image update\n!");
			cwread_image_data_request(0,cwrmsg->msgelems,cwrmsg->msgelems_len);


			cwsend_image_data_response(wtpman->conn,cwrmsg->seqnum,CW_RESULT_SUCCESS);

			send_image_file(wtpman->conn,"/tftpboot/c1130-k9w7-tar.default");
		
	//		send_image_file(wtpman->conn,"/home/tube/Downloads/c1130-k9w7-tar.123-8.JEA3.tar");
//			send_image_file(wtpman->conn,"/home/tube/Downloads/c1130-k9w8-tar.124-25e.JAP.tar");
	//		send_image_file(wtpman->conn,"/home/tube/Downloads/c1130-rcvk9w8-tar.124-25e.JAP.tar");
		
			break;
	}


	
	printf("WTP is joined now\n");

/*
	struct radioinfo * radioinfo;


	
	cwrmsg = wtpman_wait_for_message(wtpman,timer);
	if (!cwrmsg){
		cw_dbg(DBG_CW_MSG_ERR,"No config or update request from %s after %d seconds, WTP died.",
			sock_addr2str(&wtpman->conn->addr),wtpman->conn->wait_join);
			wtpman_remove(wtpman);
		return;
	}	

	printf("CWR TYPE %d\n",cwrmsg->type);
	exit(0);

*/

//	cwread_configuration_status_request(&wtpman->wtpinfo,cwrmsg->msgelems, cwrmsg->msgelems_len);
//	cwsend_conf_status_response(wtpman->conn,cwrmsg->seqnum,result_code,&radioinfo,acinfo,&wtpman->wtpinfo);

	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
	cw_dbg(DBG_ALL,"WTP conf_status\n%s",wtpinfostr);

	printf("Run run run run run\n");
	wtpman_run_run(wtpman);
	exit(0);

/*
int ii;
for (ii=0; ii<3; ii++){
	cwrmsg = wtpman_wait_for_message(wtpman,timer);
	if (cwrmsg){
		if (cwrmsg->type == CWMSG_CHANGE_STATE_EVENT_REQUEST){
			cwread_change_state_event_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);
			cwsend_change_state_event_response(wtpman->conn,cwrmsg->seqnum,wtpman->wtpinfo.radioinfo);

		}
	}
}
*/

	wtpman_run_run(wtpman);

exit(0);


	if (cwrmsg->type==CWMSG_IMAGE_DATA_REQUEST){
		cwread_image_data_request(0,cwrmsg->msgelems,cwrmsg->msgelems_len);
		cwsend_image_data_response(wtpman->conn,cwrmsg->seqnum,CW_RESULT_FAILURE);
	}






/*		
	printf("Sending image file\n");
	send_image_file(wtpman->conn,"/home/tube/Downloads/c1130-rcvk9w8-tar.124-25e.JAO5.tar");
	printf("Back from sending image file\n");
*/
exit(0);


	timer = cw_timer_start(30);
	cwrmsg = wtpman_wait_for_message(wtpman,timer);

	if (cwrmsg)
		printf("I have got a message of type %d\n",cwrmsg->type);

	if (cwrmsg->type==CWMSG_IMAGE_DATA_REQUEST){
		cwread_image_data_request(0,cwrmsg->msgelems,cwrmsg->msgelems_len);
		cwsend_image_data_response(wtpman->conn,cwrmsg->seqnum,CW_RESULT_FAILURE);
	}
	


	exit(0);




//	char wtpinfostr[8192];
//	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
//	cw_log_debug0("WTP joined\n%s",wtpinfostr);

//exit(0);

	int msg_counter = 0;
	while(1){
		struct cwrmsg * cwrmsg = conn_get_message(wtpman->conn);






		if (!cwrmsg) {
			msg_counter++;
			if (msg_counter < CAPWAP_ECHO_INTERVAL *2 ) 
				continue;
			
		//	cw_log_debug0("WTP died");
			wtpman_remove(wtpman);
			return;
		}


/*
if (cwrmsg->type == CWMSG_CONFIGURATION_STATUS_REQUEST){
	process_conf_status_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);
{
	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
	cw_dbg(DBG_CW_INFO,"Join request gave us the follwing WTP Info:\n%s",wtpinfostr);

	cwsend_conf_status_response(wtpman->conn,cwrmsg->seqnum,result_code,&radioinfo,acinfo,&wtpman->wtpinfo);
}


}
*/
		msg_counter=0;


		if (cwrmsg->type == CWMSG_ECHO_REQUEST){
			cwsend_echo_response(wtpman->conn,cwrmsg->seqnum,wtpman->wtpinfo.radioinfo);
		}
//		printf("Got msg: %i\n",cwrmsg->type);

	}




//	process_discovery(wtpman,cwrmsg);
	exit(0);


	printf("Messag gote ssl\n");

	printf("Precdeis sssl\n");

	printf ("SEQNUM REC: %i\n",cwrmsg->seqnum);

	printf("procdis ende\n");


	exit(0);




	while (1) 
	{
	/*
		sem_wait(&wtpman->q_sem);
		int qrpos = wtpman->qrpos+1;
		if (qrpos==WTPMAN_QSIZE)
			qrpos=0;
		wtpman->qrpos=qrpos;

		uint8_t * packet = wtpman->q[qrpos]+4;
		int len = *( (uint32_t*)(wtpman->q[qrpos]));
*/
		uint8_t * packet = conn_q_get_packet(wtpman->conn);
//		int len = *( (uint32_t*)(packet));


//		conn_process_packet(wtpman->conn,packet+4,len);
		free(packet);
	}
}

void wtpman_destroy(struct wtpman * wtpman)
{
	if ( wtpman->conn)
		conn_destroy(wtpman->conn);
	free (wtpman);
}



struct wtpman * wtpman_create(int socklistindex,struct sockaddr * srcaddr)
{
	struct wtpman * wtpman;
	wtpman = malloc(sizeof(struct wtpman));
	if (!wtpman)
		return 0;
	memset (wtpman,0,sizeof(struct wtpman));

	int sockfd = socklist[socklistindex].reply_sockfd;

	wtpman->conn=conn_create(sockfd,srcaddr,100);
	if (!wtpman->conn){
		wtpman_destroy(wtpman);
		return NULL;
	}

	return wtpman;
}


void wtpman_addpacket(struct wtpman * wtpman,uint8_t *packet,int len)
{
	conn_q_add_packet(wtpman->conn,packet,len);
}


void wtpman_lw_addpacket(struct wtpman *wtpman, uint8_t *packet, int len)
{
//	uint8_t * m = packet+12;
//	int l = LWTH_GET_LENGTH(packet+6);

	uint8_t * msg = packet+12;


	int msgtype = LWMSG_GET_TYPE(msg);	
	int msglen = LWMSG_GET_LEN(msg);	
	printf ("Type is %d, Len is %d\n",msgtype,msglen);
	
	uint8_t * msgdata = LWMSG_GET_DATA(msg);

/*
	int c=0; 
	while (c < msglen){
		int eltype = LWMSGELEM_GET_TYPE(data);
		int ellen = LWMSGELEM_GET_LEN(data);
		printf ("ELEM TYPE: %d, LEN: %d\n",eltype,ellen);
		c+=ellen+3;
		data=data+ellen+3;
	}	

*/

	uint8_t * data;

	lw_foreach_msgelem(data,msgdata,msglen){
		int eltype = LWMSGELEM_GET_TYPE(data);
		int ellen = LWMSGELEM_GET_LEN(data);
		uint8_t * eldata = LWMSGELEM_GET_DATA(data);

		wtpinfo_lwreadelem_wtp_descriptor(&wtpman->wtpinfo,eltype,eldata,ellen);

		printf ("ELEM TYPE: %d, LEN: %d\n",eltype,ellen);

	}


	char wi[4096];
	wtpinfo_print(wi,&wtpman->wtpinfo);
	printf ("WTPINFO: \n%s\n",wi);


	
//	char buffer[2048];
//	struct lwmsg lwmsg;
//	lwmsg_init(&lwmsg, buffer,conf_macaddress,LWMSG_DISCOVERY_RESPONSE,conn_get_next_seqnum(wtpman->conn));
	
//	conn_send_packet(wtpman->conn,buffer,60);




}


void wtpman_start(struct wtpman * wtpman,int dtlsmode)
{
	if ( dtlsmode ){
		cw_dbg(DBG_CW_INFO,"Starting wtpman in dtls mode");
		pthread_create (&wtpman->thread, NULL, (void *) &wtpman_run, (void *) wtpman);
	}
	else{
		cw_dbg(DBG_CW_INFO,"Starting wtpman in non-dtls mode");
		pthread_create (&wtpman->thread, NULL, (void *) &wtpman_run_discovery, (void *) wtpman);
	}
}


void wtpman_lw_start(struct wtpman * wtpman)
{

}


