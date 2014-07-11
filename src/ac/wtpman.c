#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <arpa/inet.h>

#include "wtplist.h"

#include "capwap.h"
#include "socklist.h"

#include "conn.h"
#include "wtpman.h"
#include "conf.h"
#include "cw_log.h"

#include "dtls.h"



#include "conf.h"




#include <errno.h>

#include "capwap.h"



ACIPLIST * get_aciplist();
struct ac_info * get_acinfo();


static void process_discovery(struct wtpman * wtpman, struct cwrmsg * cwrmsg)
//uint8_t * hdr, uint8_t *msg, int len)
{
	printf("process_discovery called\n");
	printf("cwmsg: %p\n",cwrmsg);
	printf("Message Type: %p\n",&cwrmsg->type);
	printf("Message Type: %d\n",cwrmsg->type);
	printf("WBID %d\n",cwrmsg->wbid);

	switch (cwrmsg->type){
		case CWMSG_DISCOVERY_REQUEST:
			printf ("got a discovery requesti DISC\n");
			printf("MSGELEMS: %p\n",cwrmsg->msgelems);
			printf("Seqnum %d\n",cwrmsg->seqnum);
			printf("RIIIID: %d\n",cwrmsg->rid);
			struct radioinfo radioinfo;
			radioinfo.rid = cwrmsg->rid;
			radioinfo.rmac = cwrmsg->rmac;
			struct ac_info acinfo;
//			capwap_init_acinfo(&acinfo);
			process_discovery_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);

			wtpinfo_print(&wtpman->wtpinfo);

		//	struct radioinfo radioinfo;
			cwsend_discovery_response(wtpman->conn,cwrmsg->seqnum,&radioinfo,&acinfo,&wtpman->wtpinfo);			
			break;
		case CWMSG_JOIN_REQUEST:
			printf ("Join message!!!!!!!!!!!!!!!!\n");
			process_join_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);
			wtpinfo_print(&wtpman->wtpinfo);

			exit(1);
			break;
		default:
			printf ("Unknown message\n");
			exit(1);
			break;

	}


}

static void wtpman_remove(struct wtpman * wtpman)
{
	wtplist_lock();
	wtplist_remove(wtpman);
	wtplist_unlock();
	wtpman_destroy(wtpman);
}

static void wtpman_run_discovery(void *arg)
{
	struct wtpman * wtpman = (struct wtpman *)arg;
	
	struct cwrmsg * cwrmsg;
//	do {
		cwrmsg = conn_get_message(wtpman->conn);
//	}while (!cwrmsg);


	if ( !cwrmsg)
	{
		wtpman_remove(wtpman);
		return;
	}

	if (cwrmsg->type==CWMSG_DISCOVERY_REQUEST){	
		process_discovery_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);



		struct radioinfo radioinfo;
		radioinfo.rid = cwrmsg->rid;
		radioinfo.rmac = cwrmsg->rmac;
		struct ac_info * acinfo = get_acinfo();

//		wtpinfo_print(&wtpman->wtpinfo);

		cwsend_discovery_response(wtpman->conn,cwrmsg->seqnum,&radioinfo,acinfo,&wtpman->wtpinfo);			
	}

	wtpman_remove(wtpman);
}


static void wtpman_run(void *arg)
{
	struct wtpman * wtpman = (struct wtpman *)arg;
	struct cwrmsg * cwrmsg = conn_get_message(wtpman->conn);


	if (socklist[wtpman->socklistindex].type != SOCKLIST_UNICAST_SOCKET){
		cw_log_debug0("Dropping connection to non-unicast socket");
		wtpman_remove(wtpman);
		return;
	}

	cw_log_debug0("Establishing DTLS connection");
#ifdef WITH_DTLS

	if (!conf_dtls_psk){
		cw_log(LOG_ERR,"Cant' establish DTLS connection, no psk set in config file");
		wtpman_remove(wtpman);
		return;
	}
	wtpman->conn->dtls_psk=conf_dtls_psk;
	wtpman->conn->dtls_psk_len=strlen(conf_dtls_psk);
	wtpman->conn->dtls_cipher=CAPWAP_CIPHER;

	if ( !dtls_accept(wtpman->conn) ){
		cw_log_debug0("Error establishing DTLS connection");
		wtpman_remove(wtpman);
		return;
	}
#endif	
	cw_log_debug0("DTLS Session established with");
	cwrmsg = conn_get_message(wtpman->conn);
//	printf("Seqnum: %i\n",cwrmsg->seqnum);

	if (cwrmsg->type != CWMSG_JOIN_REQUEST){
		cw_log_debug0("Join request expected but got %i",cwrmsg->type);
		wtpman_remove(wtpman);
		return;

	}
	process_join_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);

	struct radioinfo radioinfo;
	radioinfo.rid = cwrmsg->rid;
	radioinfo.rmac = cwrmsg->rmac;
	struct ac_info * acinfo = get_acinfo();

//	printf("ACN: %s\n",acinfo->ac_name);

	int result_code = 1;
	cwsend_join_response(wtpman->conn,cwrmsg->seqnum,result_code,&radioinfo,acinfo,&wtpman->wtpinfo);

	cw_log_debug0("WTP joined");

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

		msg_counter=0;


		if (cwrmsg->type = CWMSG_ECHO_REQUEST){
			cwsend_echo_response(wtpman->conn,cwrmsg->seqnum,wtpman->wtpinfo.radioinfo);
		}
		printf("Got msg: %i\n",cwrmsg->type);

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

//	wtpman->conn=conn_create(sockfd,srcaddr,process_discovery,wtpman,100);
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



void wtpman_start(struct wtpman * wtpman,int dtlsmode)
{
	if ( dtlsmode )
		pthread_create (&wtpman->thread, NULL, (void *) &wtpman_run, (void *) wtpman);
	else
		pthread_create (&wtpman->thread, NULL, (void *) &wtpman_run_discovery, (void *) wtpman);
}
