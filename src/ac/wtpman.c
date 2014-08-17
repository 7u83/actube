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


#include <errno.h>

#include "capwap.h"

#include "cw_util.h"


/* macro to convert our client ip to a string */
#define CLIENT_IP (sock_addrtostr((struct sockaddr*)&wtpman->conn->addr, (char[64]){0},64))




static struct cwrmsg * conn_wait_for_message(struct conn * conn, time_t timer)
{
	struct cwrmsg * cwrmsg;
	do {
		cwrmsg = conn_get_message(conn);
		if (cwrmsg){
			if (cwrmsg->type == CWMSG_ECHO_REQUEST){
				printf("Echo reponse\n");
				cwsend_echo_response(conn,cwrmsg->seqnum,0);
				printf("continue\n");
				cwrmsg=0;
				continue;
			}
		}


		if (!cwrmsg && conn->dtls_error)
			return (struct cwrmsg*)EOF;
		if (!cwrmsg && cw_timer_timeout(timer)) 
			return NULL;

	}while(!cwrmsg);

	printf("Nw here\n");
	cw_dbg(DBG_CW_MSG,"Received message from %s, type=%d - %s"
		,sock_addr2str(&conn->addr),cwrmsg->type,cw_msgtostr(cwrmsg->type));

	return cwrmsg;
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


	do{
		data.len = fread(buffer,1,sizeof(buffer),infile);
		if (feof(infile))
			data.type=2;
		else	
			data.type=1;
		printf("Send img data request\n");
		cwsend_image_data_request(conn,&data,0);
		cwrmsg = conn_get_response(conn);
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


ACIPLIST * get_aciplist();
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

	if ( !cwrmsg || cwrmsg == EOF )
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


	process_discovery_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);



	struct radioinfo radioinfo;
	radioinfo.rid = cwrmsg->rid;
	memcpy(radioinfo.rmac, cwrmsg->rmac,8);
	radioinfo.rmac[0]=0;

	struct ac_info * acinfo = get_acinfo();



	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
	cw_dbg(DBG_CW_INFO,"Discovery request gave us the follwing WTP Info:\n%s",wtpinfostr);


	cwsend_discovery_response(wtpman->conn,cwrmsg->seqnum,&radioinfo,acinfo,&wtpman->wtpinfo);			

	wtpman_remove(wtpman);
}


static void wtpman_run(void *arg)
{
	struct wtpman * wtpman = (struct wtpman *)arg;
	struct cwrmsg * cwrmsg = conn_get_message(wtpman->conn);


	if (socklist[wtpman->socklistindex].type != SOCKLIST_UNICAST_SOCKET){
		cw_dbg(DBG_DTLS,"Dropping connection from %s to non-unicast socket", CLIENT_IP);
		wtpman_remove(wtpman);
		return;
	}

	/* start DTLS handshake */
	cw_dbg(DBG_DTLS,"Establishing DTLS session with %s",CLIENT_IP);

	time_t timer = cw_timer_start(wtpman->conn->wait_dtls);


	wtpman->conn->dtls_cipher=CAPWAP_CIPHER;

	/* setup DTSL certificates */
	int dtls_ok=0;
	if (conf_sslkeyfilename && conf_sslcertfilename){
		wtpman->conn->dtls_key_file = conf_sslkeyfilename;
		wtpman->conn->dtls_cert_file = conf_sslcertfilename;
		wtpman->conn->dtls_key_pass = conf_sslkeypass;
		wtpman->conn->dtls_verify_peer = conf_dtls_verify_peer;
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
		wtpman_remove(wtpman);
		return;
	}

	/* try to accept the connection */
	if ( !dtls_accept(wtpman->conn) ){
		cw_dbg(DBG_DTLS,"Error establishing DTLS session with %s",CLIENT_IP);
		wtpman_remove(wtpman);
		return;
	}
	
	cw_dbg(DBG_DTLS,"DTLS session established with %s, cipher=%s", CLIENT_IP,dtls_get_cipher(wtpman->conn));
	/* DTLS handshake done */


	timer = cw_timer_start(wtpman->conn->wait_join);

	/* In join state, wait for join request */
	cwrmsg = wtpman_wait_for_message(wtpman,timer);

	if (!cwrmsg){
		cw_dbg(DBG_CW_MSG_ERR,"No join request from %s after %d seconds, WTP died.",
			sock_addr2str(&wtpman->conn->addr),wtpman->conn->wait_join);
			wtpman_remove(wtpman);
		return;
	}	

	if (cwrmsg == (struct cwrmsg*)EOF){
		cw_dbg(DBG_CW_MSG_ERR,"DTLS connection closed while waiting for join request from %s.",CLIENT_IP);
		wtpman_remove(wtpman);
		return;
	}

	/* the received message MUST be a join request */	

	if (cwrmsg->type != CWMSG_JOIN_REQUEST){
		cw_dbg(DBG_CW_MSG_ERR,"Join request expected from %s, but got %i",CLIENT_IP,cwrmsg->type);
		wtpman_remove(wtpman);
		return;
	}

	process_join_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);

	{
	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
	cw_dbg(DBG_CW_INFO,"Join request gave us the follwing WTP Info:\n%s",wtpinfostr);
	}


	struct radioinfo radioinfo;
	radioinfo.rid = cwrmsg->rid;
	memcpy (radioinfo.rmac, cwrmsg->rmac,8);
	struct ac_info * acinfo = get_acinfo();


	int result_code = 0;
	cw_dbg(DBG_CW_MSG,"Sending join response to %s",CLIENT_IP);
	cwsend_join_response(wtpman->conn,cwrmsg->seqnum,result_code,&radioinfo,acinfo,&wtpman->wtpinfo);
	cw_log(LOG_INFO,"WTP joined, Name = %s, Location = %s, IP = %s",
		wtpman->wtpinfo.name,wtpman->wtpinfo.location,
		sock_addr2str(&wtpman->conn->addr));

	/* here the WTP has joined */

	
	cwrmsg = wtpman_wait_for_message(wtpman,timer);
	if (!cwrmsg){
		cw_dbg(DBG_CW_MSG_ERR,"No config or update request from %s after %d seconds, WTP died.",
			sock_addr2str(&wtpman->conn->addr),wtpman->conn->wait_join);
			wtpman_remove(wtpman);
		return;
	}	

	cwread_configuration_status_request(&wtpman->wtpinfo,cwrmsg->msgelems, cwrmsg->msgelems_len);
	cwsend_conf_status_response(wtpman->conn,cwrmsg->seqnum,result_code,&radioinfo,acinfo,&wtpman->wtpinfo);

	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
	cw_dbg(DBG_ALL,"WTP conf_status\n%s",wtpinfostr);



	cwrmsg = wtpman_wait_for_message(wtpman,timer);

	if (cwrmsg){
		if (cwrmsg->type == CWMSG_CHANGE_STATE_EVENT_REQUEST){
			int rc = cwread_change_state_event_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);
			printf("Change state RC: %d\n",rc);

		}
	}

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
			
			cw_log_debug0("WTP died");
			wtpman_remove(wtpman);
			return;
		}



if (cwrmsg->type = CWMSG_CONFIGURATION_STATUS_REQUEST){
	process_conf_status_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);
{
	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
	cw_dbg(DBG_CW_INFO,"Join request gave us the follwing WTP Info:\n%s",wtpinfostr);

	cwsend_conf_status_response(wtpman->conn,cwrmsg->seqnum,result_code,&radioinfo,acinfo,&wtpman->wtpinfo);
}


}

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
		int len = *( (uint32_t*)(packet));


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
	uint8_t * m = packet+12;
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


	
	char buffer[2048];
	struct lwmsg lwmsg;
	lwmsg_init(&lwmsg, buffer,conf_macaddress,LWMSG_DISCOVERY_RESPONSE,conn_get_next_seqnum(wtpman->conn));
	
	conn_send_packet(wtpman->conn,buffer,60);




}


void wtpman_start(struct wtpman * wtpman,int dtlsmode)
{
	if ( dtlsmode ){
		cw_log_debug1("Starting wtpman in dtls mode");
		pthread_create (&wtpman->thread, NULL, (void *) &wtpman_run, (void *) wtpman);
	}
	else{
		cw_log_debug1("Starting wtpman in non-dtls mode");
		pthread_create (&wtpman->thread, NULL, (void *) &wtpman_run_discovery, (void *) wtpman);
	}
}


void wtpman_lw_start(struct wtpman * wtpman)
{

}


