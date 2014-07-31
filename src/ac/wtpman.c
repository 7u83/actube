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


/* macro to convert our client ip to a string */
#define CLIENT_IP (sock_addrtostr((struct sockaddr*)&wtpman->conn->addr, (char[64]){0},64))



ACIPLIST * get_aciplist();
struct ac_info * get_acinfo();

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

	printf("con get message\n");

//	do {
		cwrmsg = conn_get_message(wtpman->conn);
//	}while (!cwrmsg);

	printf("cwrmsg = %p\n",cwrmsg);
	printf("RID: %d, WBID %d\n",cwrmsg->rid,cwrmsg->wbid);


	if ( !cwrmsg)
	{
		wtpman_remove(wtpman);
		return;
	}


	printf("cwrmswg type = %08X\n",cwrmsg->type);

	if (cwrmsg->type==CWMSG_DISCOVERY_REQUEST){	
		process_discovery_request(&wtpman->wtpinfo,cwrmsg->msgelems,cwrmsg->msgelems_len);



		struct radioinfo radioinfo;
		radioinfo.rid = cwrmsg->rid;
		memcpy(radioinfo.rmac, cwrmsg->rmac,8);
		radioinfo.rmac[0]=0;
//		cwrmsg->rmac[0]=0;

//		printf("The RID %d\n",radioinfo.rid);
/*		int i;
		for (i=0; i<8; i++){
			printf("Rec RMAC: %02x\n",cwrmsg->rmac[i]);

		}
*/
	//	radioinfo.rmac=0;


		struct ac_info * acinfo = get_acinfo();



		char wtpinfostr[8192];
		wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
		cw_log_debug2("Discovery request gave us the follwing WTP Info:\n%s",wtpinfostr);

//		wtpinfo_print(&wtpman->wtpinfo);

		cwsend_discovery_response(wtpman->conn,cwrmsg->seqnum,&radioinfo,acinfo,&wtpman->wtpinfo);			
	}

	wtpman_remove(wtpman);
}



static void wtpman_run(void *arg)
{
	struct wtpman * wtpman = (struct wtpman *)arg;
	struct cwrmsg * cwrmsg = conn_get_message(wtpman->conn);

	printf("Running DTLS\n");

	if (socklist[wtpman->socklistindex].type != SOCKLIST_UNICAST_SOCKET){
		cw_log_debug0("Dropping connection from %s to non-unicast socket", CLIENT_IP);
		wtpman_remove(wtpman);
		return;
	}

	cw_log_debug0("Establishing DTLS connection from %s",CLIENT_IP);

#ifdef WITH_DTLS
	if (!conf_dtls_psk){
		cw_log(LOG_ERR,"Cant' establish DTLS connection, no psk set in config file");
		wtpman_remove(wtpman);
		return;
	}
	wtpman->conn->dtls_psk=conf_dtls_psk;
	wtpman->conn->dtls_psk_len=strlen(conf_dtls_psk);
	wtpman->conn->dtls_cipher=CAPWAP_CIPHER;

	printf ("Goin to dtls accept\n");

	if ( !dtls_accept(wtpman->conn) ){
		cw_log_debug0("Error establishing DTLS connection from %s",CLIENT_IP);
		wtpman_remove(wtpman);
		return;
	}
	
	printf("DTLS Done\n");
#endif	
//	const struct sockaddr *sa, char *s, size_t maxlen

	cw_log_debug0("DTLS Session established with %s", CLIENT_IP);


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
	memcpy (radioinfo.rmac, cwrmsg->rmac,8);
	struct ac_info * acinfo = get_acinfo();

//	printf("ACN: %s\n",acinfo->ac_name);

	int result_code = 1;
	cwsend_join_response(wtpman->conn,cwrmsg->seqnum,result_code,&radioinfo,acinfo,&wtpman->wtpinfo);

	cw_log_debug0("WTP joined %s,%s",wtpman->wtpinfo.name,wtpman->wtpinfo.location);

	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);

	cw_log_debug0("WTP joined\n%s",wtpinfostr);

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


		if (cwrmsg->type == CWMSG_ECHO_REQUEST){
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


void wtpman_lw_addpacket(struct wtpman *wtpman, uint8_t *packet, int len)
{
	uint8_t * m = packet+12;
	int l = LWTH_GET_LENGTH(packet+6);

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


