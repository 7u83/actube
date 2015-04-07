#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <arpa/inet.h>

#include "wtplist.h"

#include "capwap/capwap.h"
#include "capwap/sock.h"
#include "socklist.h"

#include "capwap/conn.h"
#include "wtpman.h"
#include "conf.h"
#include "capwap/cw_log.h"


#include "conf.h"

#include "capwap/lwmsg.h"
#include "capwap/lwapp.h"


#include <errno.h>

#include "capwap/capwap_80211.h"
#include "capwap/capwap_cisco.h"

#include "capwap/cw_util.h"

#include "capwap/capwap_items.h"
#include "ac.h"

#include "capwap/dtls.h"


extern struct cw_actiondef capwap_actions;


/* macro to convert our client ip to a string */
//#define CLIENT_IP (sock_addrtostr((struct sockaddr*)&wtpman->conn->addr, (char[64]){0},64))

#define CLIENT_IP (sock_addr2str(&wtpman->conn->addr))

void conn_handle_change_state_event_request(struct conn *conn)
{
}

struct rh_param {
	struct conn *conn;
	int *msglist;

};

static int conn_rh(void *param)
{
	struct rh_param *p = (struct rh_param *) param;
	int i;
	int *msglist = p->msglist;


	printf("Param %p\n", param);


	for (i = 0; msglist[i] != -1; i++) {
		if (msglist[i] == p->conn->cwrmsg.type)
			return 0;

	}
	/* unexpected response here */
	cw_log(LOG_ERR, "Unexpected message from %s", sock_addr2str(&p->conn->addr));
	cwsend_unknown_response(p->conn, p->conn->cwrmsg.seqnum, p->conn->cwrmsg.type);
	return 1;
}


struct ac_info *get_acinfo();

static void wtpman_remove(struct wtpman *wtpman)
{
	wtplist_lock();
	wtplist_remove(wtpman);
	wtplist_unlock();
	wtpman_destroy(wtpman);
}



int check_discovery(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len)
{
	printf("Discovery END!!!\n");
	conn->capwap_state=CW_STATE_NONE;
	return 0;

}

static void wtpman_run_discovery(void *arg)
{

	struct wtpman *wtpman = (struct wtpman *) arg;
	struct cwrmsg *cwrmsg;


	time_t timer = cw_timer_start(10);

	extern cw_actionlist_in_t the_tree;

	wtpman->conn->capwap_state = CW_STATE_DISCOVERY;
	wtpman->conn->actions = &capwap_actions;

/*
	cw_actionlist_in_set_msg_end_callback(capwap_actions.in, CW_STATE_DISCOVERY,
					      CW_MSG_DISCOVERY_REQUEST, check_discovery);

*/


	wtpman->conn->local = ac_config;
	wtpman->conn->remote = cw_itemstore_create();

	while (!cw_timer_timeout(timer)
	       && wtpman->conn->capwap_state == CW_STATE_DISCOVERY) {
		cw_read_messages(wtpman->conn);
	}

	struct cw_item *wn = cw_itemstore_get(wtpman->conn->remote, CW_ITEM_WTP_NAME);

	if (wn) {
		printf("WTP Name: %s\n", wn->data);
	}

	wtpman_remove(wtpman);
	return;






	timer = cw_timer_start(10);
	cwrmsg = wtpman_wait_for_message(wtpman, timer);

	if (!cwrmsg) {
		cw_dbg(DBG_MSG_ERR,
		       "No complete message from %s received after %d seconds", CLIENT_IP,
		       10);
		wtpman_remove(wtpman);
		return;
	}


	if (cwrmsg->type != CW_MSG_DISCOVERY_REQUEST) {
		cw_dbg(DBG_MSG_ERR,
		       "Invalid message in discovery state from %s, type=%s - %s ",
		       CLIENT_IP, cwrmsg->type, cw_msgtostr(cwrmsg->type));
		wtpman_remove(wtpman);
		return;
	}

	cwread_discovery_request(&wtpman->wtpinfo, cwrmsg->msgelems,
				 cwrmsg->msgelems_len);


//      printf("RMAC-LEN:%d\n",cwrmsg->rmac[0]);

//printf("HW: %s\n",sock_hwaddr2str(bstr_data(cwrmsg->rmac),bstr_len(cwrmsg->rmac)));



	conn_detect_capwap(wtpman->conn, &wtpman->wtpinfo);

	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr, &wtpman->wtpinfo);
	cw_dbg(DBG_CW_INFO, "Discovery request gave us the follwing WTP Info:\n%s",
	       wtpinfostr);


	struct radioinfo radioinfo;
	radioinfo.rid = cwrmsg->rid;
	radioinfo.rmac = 0;	//&cwrmsg->rmac;

//      memcpy(radioinfo.rmac, cwrmsg->rmac,8);
//      radioinfo.rmac[0]=0;

	struct ac_info *acinfo = get_acinfo();



/*	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
	cw_dbg(DBG_CW_INFO,"Discovery request gave us the follwing WTP Info:\n%s",wtpinfostr);
*/

	cwsend_discovery_response(wtpman->conn, cwrmsg->seqnum, &radioinfo, acinfo,
				  &wtpman->wtpinfo);
	wtpman_remove(wtpman);
}


static void wtpman_run_run(void *arg)
{


}



static int wtpman_establish_dtls(void *arg)
{
	struct wtpman *wtpman = (struct wtpman *) arg;

	/* setup cipher */
	wtpman->conn->dtls_cipher = conf_sslcipher;

	/* setup DTSL certificates */
	int dtls_ok = 0;
	if (conf_sslkeyfilename && conf_sslcertfilename) {


		wtpman->conn->dtls_key_file = conf_sslkeyfilename;
		wtpman->conn->dtls_cert_file = conf_sslcertfilename;
		wtpman->conn->dtls_key_pass = conf_sslkeypass;
		wtpman->conn->dtls_verify_peer = conf_dtls_verify_peer;
		cw_dbg(DBG_DTLS, "DTLS - Using key file %s", wtpman->conn->dtls_key_file);
		cw_dbg(DBG_DTLS, "DTLS - Using cert file %s",
		       wtpman->conn->dtls_cert_file);
		dtls_ok = 1;
	}

	/* setup DTLS psk */
	if (conf_dtls_psk) {
		wtpman->conn->dtls_psk = conf_dtls_psk;
		wtpman->conn->dtls_psk_len = strlen(conf_dtls_psk);
		dtls_ok = 1;
	}

	if (!dtls_ok) {
		cw_log(LOG_ERR,
		       "Can't establish DTLS session, neither psk nor certs set in config file.");
		return 0;
	}

	/* try to accept the connection */
	if (!dtls_accept(wtpman->conn)) {
		cw_dbg(DBG_DTLS, "Error establishing DTLS session with %s", CLIENT_IP);
		return 0;
	}

	cw_dbg(DBG_DTLS, "DTLS session established with %s, cipher=%s", CLIENT_IP,
	       dtls_get_cipher(wtpman->conn));
	/* DTLS handshake done */

	int cert_len;
	struct dtls_ssl_cert cert;

	FILE *f;
	f = fopen("pcert.der", "wb");

	printf("Calling peers cert\n");
	cert = dtls_get_peers_cert(wtpman->conn, 0);
	printf("Back from peers cert\n");

	printf("Have Peers Cert: %p, %d\n", cert.data, cert.size);
	fwrite(cert.data, 1, cert.size, f);


	return 1;
}

static int wtpman_join(void *arg, time_t timer)
{
	struct wtpman *wtpman = (struct wtpman *) arg;

	extern cw_actionlist_in_t the_tree;
	wtpman->conn->actions = &capwap_actions;
	wtpman->conn->capwap_state = CW_STATE_JOIN;

	wtpman->conn->capwap_state = CW_STATE_JOIN;
	wtpman->conn->actions = &capwap_actions;

//      wtpman->conn->itemstore = cw_itemstore_create();

	wtpman->conn->local = ac_config;
	wtpman->conn->remote = cw_itemstore_create();

	while (!cw_timer_timeout(timer) && wtpman->conn->capwap_state == CW_STATE_JOIN) {
		int rc = cw_read_messages(wtpman->conn);
		if (rc < 0) {
			break;
		}
	}

	if (wtpman->conn->capwap_state==CW_STATE_JOIN){
		cw_dbg(DBG_MSG_ERR, "No join request from %s after %d seconds, WTP died.",
		       sock_addr2str(&wtpman->conn->addr), wtpman->conn->wait_dtls);

		return 0;
	}
	

	return 1;


}


static int wtpman_send_image_file(struct wtpman *wtpman, struct cwrmsg *cwrmsg)
{
	struct cwimage_data data;
	memset(&data, 0, sizeof(struct cwimage_data));
	uint8_t id[1025];
	data.identifier = id;
	char filename[2048];
	id[0] = 0;


	cw_read_image_data_request(&data, cwrmsg->msgelems, cwrmsg->msgelems_len);
	if (!strlen(id)) {
		cw_dbg(DBG_MSG_ERR, "No image identifier in image data request");
		cw_send_image_data_response(wtpman->conn, cwrmsg->seqnum,
					    CW_RESULT_FAILURE);
		return 0;
	}

	sprintf(filename, "%s/%s", conf_image_dir, id);

	FILE *infile;
	infile = fopen(filename, "rb");
	if (infile) {
		cw_send_image_data_response(wtpman->conn, cwrmsg->seqnum,
					    CW_RESULT_SUCCESS);
		cw_log(LOG_INFO, "Sending image file %s to %s", filename,
		       sock_addr2str(&wtpman->conn->addr));
		cw_send_image_file(wtpman->conn, infile);
		return 1;
	}

	cw_log(LOG_ERR, "Can't open image file %s:%s", filename, strerror(errno));
	cw_send_image_data_response(wtpman->conn, cwrmsg->seqnum, CW_RESULT_FAILURE);

	return 0;

}

static void wtpman_run(void *arg)
{
	struct wtpman *wtpman = (struct wtpman *) arg;
	struct cwrmsg *cwrmsg;	// = conn_get_message(wtpman->conn);

	wtpman->conn->seqnum = 0;

	/* reject connections to our multi- or broadcast sockets */
	if (socklist[wtpman->socklistindex].type != SOCKLIST_UNICAST_SOCKET) {
		cw_dbg(DBG_DTLS, "Dropping connection from %s to non-unicast socket.",
		       CLIENT_IP);
		wtpman_remove(wtpman);
		return;
	}


	time_t timer = cw_timer_start(wtpman->conn->wait_dtls);

	/* establish dtls session */
	if (!wtpman_establish_dtls(wtpman)) {
		wtpman_remove(wtpman);
		return;
	}


	/* dtls is established, goto join state */
	if (!wtpman_join(wtpman, timer)) {
		wtpman_remove(wtpman);
		return;
	}




	/* here the WTP has joined, now we assume an image data request  
	   or an configuration status request. Nothing else. 
	   State is Image update 
	 */

	
	while (!cw_timer_timeout(timer) && wtpman->conn->capwap_state == CW_STATE_CONFIGURE) {
		int rc = cw_read_messages(wtpman->conn);
		if (rc < 0) {
			break;
		}
	}






	exit(0);








/*





	do {
		int cfg_status_msgs[] =
		    { CW_MSG_IMAGE_DATA_REQUEST, CW_MSG_CONFIGURATION_STATUS_REQUEST,
	-1 };
		cwrmsg = conn_wait_for_request(wtpman->conn, cfg_status_msgs, timer);

		if (!cwrmsg) {
			cw_dbg(DBG_MSG_ERR,
			       "No conf status or img data request from %s after %d seconds, WTP died.",
			       sock_addr2str(&wtpman->conn->addr),
			       wtpman->conn->wait_join);
			wtpman_remove(wtpman);
			return;
		}

		// Image data request, the WTP wants an update 
		if (cwrmsg->type == CW_MSG_IMAGE_DATA_REQUEST) {
			int rc = wtpman_send_image_file(wtpman, cwrmsg);
			if (rc) {
				wtpman_remove(wtpman);
				return;
			}

		}

	} while (cwrmsg->type != CW_MSG_CONFIGURATION_STATUS_REQUEST);



	printf("Have Masseg %d\n", cwrmsg->type);
	cwread_configuration_status_request(&wtpman->wtpinfo, cwrmsg->msgelems,
					    cwrmsg->msgelems_len);
	int result_code = 0;
	struct ac_info *acinfo = get_acinfo();
	struct radioinfo rinf;
	cwsend_conf_status_response(wtpman->conn, cwrmsg->seqnum, result_code, &rinf,
				    acinfo, &wtpman->wtpinfo);
	printf("Send the respi but sleep\n");


	printf("Next thoing\n");
	int change_status_msgs[] =
	    { CW_MSG_IMAGE_DATA_REQUEST, CW_MSG_CHANGE_STATE_EVENT_REQUEST, -1 };
	cwrmsg = conn_wait_for_request(wtpman->conn, change_status_msgs, timer);
	printf("Done\n");

	if (!cwrmsg) {
		wtpman_remove(wtpman);
		return;
	}



	switch (cwrmsg->type) {
		case CW_MSG_CHANGE_STATE_EVENT_REQUEST:
			{
				printf("Change state event\n!");
				struct radioinfo ri;
				cwsend_change_state_event_response(wtpman->conn,
								   cwrmsg->seqnum, &ri);
			}
			break;
		case CW_MSG_IMAGE_DATA_REQUEST:
			printf("Image update\n!");
			//cwread_image_data_request(0,cwrmsg->msgelems,cwrmsg->msgelems_len);


			//cwsend_image_data_response(wtpman->conn,cwrmsg->seqnum,CW_RESULT_SUCCESS);

			//send_image_file(wtpman->conn,"/tftpboot/c1130-k9w7-tar.default");

			//              send_image_file(wtpman->conn,"/home/tube/Downloads/c1130-k9w7-tar.123-8.JEA3.tar");
//                      send_image_file(wtpman->conn,"/home/tube/Downloads/c1130-k9w8-tar.124-25e.JAP.tar");
			//              send_image_file(wtpman->conn,"/home/tube/Downloads/c1130-rcvk9w8-tar.124-25e.JAP.tar");

			break;
	}



	printf("WTP is joined now\n");
*/

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

//      cwread_configuration_status_request(&wtpman->wtpinfo,cwrmsg->msgelems, cwrmsg->msgelems_len);
//      cwsend_conf_status_response(wtpman->conn,cwrmsg->seqnum,result_code,&radioinfo,acinfo,&wtpman->wtpinfo);

/*
	char wtpinfostr[8192];
	wtpinfo_print(wtpinfostr, &wtpman->wtpinfo);
	cw_dbg(DBG_ALL, "WTP conf_status\n%s", wtpinfostr);

	printf("Run run run run run\n");
	wtpman_run_run(wtpman);
	exit(0);
*/

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

/*
	wtpman_run_run(wtpman);

	exit(0);


	if (cwrmsg->type == CW_MSG_IMAGE_DATA_REQUEST) {
		cwread_image_data_request(0, cwrmsg->msgelems, cwrmsg->msgelems_len);
		cwsend_image_data_response(wtpman->conn, cwrmsg->seqnum,
					   CW_RESULT_FAILURE);
	}

*/




/*		
	printf("Sending image file\n");
	send_image_file(wtpman->conn,"/home/tube/Downloads/c1130-rcvk9w8-tar.124-25e.JAO5.tar");
	printf("Back from sending image file\n");
*/
	exit(0);


/*
	timer = cw_timer_start(30);
	cwrmsg = wtpman_wait_for_message(wtpman, timer);

	if (cwrmsg)
		printf("I have got a message of type %d\n", cwrmsg->type);

	if (cwrmsg->type == CW_MSG_IMAGE_DATA_REQUEST) {
		cwread_image_data_request(0, cwrmsg->msgelems, cwrmsg->msgelems_len);
		cwsend_image_data_response(wtpman->conn, cwrmsg->seqnum,
					   CW_RESULT_FAILURE);
	}


*/




//      char wtpinfostr[8192];
//      wtpinfo_print(wtpinfostr,&wtpman->wtpinfo);
//      cw_log_debug0("WTP joined\n%s",wtpinfostr);

//exit(0);
/*
	int msg_counter = 0;
	while (1) {
		struct cwrmsg *cwrmsg = conn_get_message(wtpman->conn);






		if (!cwrmsg) {
			msg_counter++;
			if (msg_counter < CAPWAP_ECHO_INTERVAL * 2)
				continue;

			//      cw_log_debug0("WTP died");
			wtpman_remove(wtpman);
			return;
		}

*/
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

/*
		msg_counter = 0;


		if (cwrmsg->type == CW_MSG_ECHO_REQUEST) {
			cwsend_echo_response(wtpman->conn, cwrmsg->seqnum,
					     wtpman->wtpinfo.radioinfo);
		}
*/

//              printf("Got msg: %i\n",cwrmsg->type);

}




//      process_discovery(wtpman,cwrmsg);
	

/*
	while (1) {
		uint8_t *packet = conn_q_get_packet(wtpman->conn);
//              int len = *( (uint32_t*)(packet));


//              conn_process_packet(wtpman->conn,packet+4,len);
		free(packet);
	}
*/



void wtpman_destroy(struct wtpman *wtpman)
{
	if (wtpman->conn)
		conn_destroy(wtpman->conn);
	free(wtpman);
}



struct wtpman *wtpman_create(int socklistindex, struct sockaddr *srcaddr)
{
	struct wtpman *wtpman;
	wtpman = malloc(sizeof(struct wtpman));
	if (!wtpman)
		return 0;
	memset(wtpman, 0, sizeof(struct wtpman));

	int sockfd = socklist[socklistindex].reply_sockfd;

	wtpman->conn = conn_create(sockfd, srcaddr, 100);
	if (!wtpman->conn) {
		wtpman_destroy(wtpman);
		return NULL;
	}

	return wtpman;
}


void wtpman_addpacket(struct wtpman *wtpman, uint8_t * packet, int len)
{
	conn_q_add_packet(wtpman->conn, packet, len);
}


void wtpman_lw_addpacket(struct wtpman *wtpman, uint8_t * packet, int len)
{
//      uint8_t * m = packet+12;
//      int l = LWTH_GET_LENGTH(packet+6);

	uint8_t *msg = packet + 12;


	int msgtype = LWMSG_GET_TYPE(msg);
	int msglen = LWMSG_GET_LEN(msg);
	printf("Type is %d, Len is %d\n", msgtype, msglen);

	uint8_t *msgdata = LWMSG_GET_DATA(msg);

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

	uint8_t *data;
/*
	lw_foreach_msgelem(data,msgdata,msglen){
		int eltype = LWMSGELEM_GET_TYPE(data);
		int ellen = LWMSGELEM_GET_LEN(data);
		uint8_t * eldata = LWMSGELEM_GET_DATA(data);

		wtpinfo_lwreadelem_wtp_descriptor(&wtpman->wtpinfo,eltype,eldata,ellen);

		printf ("ELEM TYPE: %d, LEN: %d\n",eltype,ellen);

	}
*/

	char wi[4096];
	wtpinfo_print(wi, &wtpman->wtpinfo);
	printf("WTPINFO: \n%s\n", wi);



//      char buffer[2048];
//      struct lwmsg lwmsg;
//      lwmsg_init(&lwmsg, buffer,conf_macaddress,LWMSG_DISCOVERY_RESPONSE,conn_get_next_seqnum(wtpman->conn));

//      conn_send_packet(wtpman->conn,buffer,60);




}


void wtpman_start(struct wtpman *wtpman, int dtlsmode)
{
	if (dtlsmode) {
		cw_dbg(DBG_CW_INFO, "Starting wtpman in DTLS mode");
		pthread_create(&wtpman->thread, NULL, (void *) &wtpman_run,
			       (void *) wtpman);
	} else {
		cw_dbg(DBG_CW_INFO, "Starting wtpman in non-dtls mode");
		pthread_create(&wtpman->thread, NULL, (void *) &wtpman_run_discovery,
			       (void *) wtpman);
	}
}


void wtpman_lw_start(struct wtpman *wtpman)
{

}
