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
#include "capwap/log.h"
#include "capwap/timer.h"



#include "capwap/lwmsg.h"
#include "capwap/lwapp.h"


#include <errno.h>

#include "capwap/capwap_80211.h"
#include "capwap/capwap_cisco.h"

#include "capwap/cw_util.h"

#include "capwap/capwap_items.h"
#include "ac.h"

#include "capwap/dtls.h"

#include "capwap/dbg.h"


extern struct cw_actiondef capwap_actions;


/* macro to convert our client ip to a string */
//#define CLIENT_IP (sock_addrtostr((struct sockaddr*)&wtpman->conn->addr, (char[64]){0},64))

#define CLIENT_IP (sock_addr2str(&wtpman->conn->addr))

void conn_handle_change_state_event_request(struct conn *conn)
{
}

/*
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
	cw_log(LOG_ERR, "Unexpected message from %s", sock_addr2str(&p->conn->addr));
	cwsend_unknown_response(p->conn, p->conn->cwrmsg.seqnum, p->conn->cwrmsg.type);
	return 1;
}
*/

/*
struct ac_info *get_acinfo();
*/

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
	conn->capwap_state = CW_STATE_NONE;
	return 0;

}

static void wtpman_run_discovery(void *arg)
{


	struct wtpman *wtpman = (struct wtpman *) arg;
	//struct cwrmsg *cwrmsg;

	struct conn * conn = wtpman->conn;


//conn->config = mbag_create();
//conn->radios = mbag_create();


	time_t timer = cw_timer_start(10);

	extern cw_actionlist_in_t the_tree;

	wtpman->conn->capwap_state = CW_STATE_DISCOVERY;
	wtpman->conn->actions = &capwap_actions;

/*
	cw_actionlist_in_set_msg_end_callback(capwap_actions.in, CW_STATE_DISCOVERY,
					      CW_MSG_DISCOVERY_REQUEST, check_discovery);

*/


	wtpman->conn->outgoing = mbag_create();
	wtpman->conn->incomming = mbag_create();

	while (!cw_timer_timeout(timer)
	       && wtpman->conn->capwap_state == CW_STATE_DISCOVERY) {
		cw_read_messages(wtpman->conn);
	}

	struct mbag_item *wn = mbag_get(wtpman->conn->incomming, CW_ITEM_WTP_NAME);

	if (wn) {
		//printf("WTP Name: %s\n", wn->data);
	}

	wtpman_remove(wtpman);
	return;

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
		cw_dbg(DBG_DTLS, "Using key file %s", wtpman->conn->dtls_key_file);
		cw_dbg(DBG_DTLS, "Using cert file %s",
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

	cert = dtls_get_peers_cert(wtpman->conn, 0);

//	printf("Have Peers Cert: %p, %d\n", cert.data, cert.size);
//	fwrite(cert.data, 1, cert.size, f);
	fclose(f);


	return 1;
}

static int wtpman_join(void *arg, time_t timer)
{
	struct wtpman *wtpman = (struct wtpman *) arg;
	struct conn * conn = wtpman->conn;

	wtpman->conn->outgoing = mbag_create();
	wtpman->conn->incomming = mbag_create();
	conn->config=conn->incomming;
//	wtpman->conn->local = ac_config;

	mbag_set_str(conn->local,CW_ITEM_AC_NAME,conf_acname);





	extern cw_actionlist_in_t the_tree;
	wtpman->conn->actions = &capwap_actions;
	wtpman->conn->capwap_state = CW_STATE_JOIN;

	wtpman->conn->capwap_state = CW_STATE_JOIN;
	wtpman->conn->actions = &capwap_actions;

//      wtpman->conn->itemstore = mbag_create();






	cw_dbg(DBG_INFO,"Join State - %s",sock_addr2str(&conn->addr));

	int rc;
	while (!cw_timer_timeout(timer) && wtpman->conn->capwap_state == CW_STATE_JOIN) {
		rc = cw_read_messages(wtpman->conn);
		if (rc < 0) {
			break;
		}
	}

	if (rc != 0  ) {
		cw_log(LOG_ERR,"Error joining WTP %s",cw_strerror(rc));
		return 0;

	}


	if (wtpman->conn->capwap_state == CW_STATE_JOIN) {
		cw_dbg(DBG_MSG_ERR, "No join request from %s after %d seconds, WTP died.",
		       sock_addr2str(&wtpman->conn->addr), wtpman->conn->wait_dtls);

		return 0;
	}


	return 1;


}

static void wtpman_image_data(struct wtpman *wtpman)
{
	struct conn * conn = wtpman->conn;

		/* Image upload */
		const char *filename =
		    mbag_get_str(conn->outgoing, CW_ITEM_IMAGE_FILENAME,NULL);
		if (!filename) {
			cw_log(LOG_ERR,
			       "Can't send image to %s. No Image Filename Item found.",
			       sock_addr2str(&conn->addr));
			return;
		}
		cw_dbg(DBG_INFO, "Sending image file '%s' to '%s'.", filename,
		       sock_addr2str(&conn->addr));
		FILE *infile = fopen(filename, "rb");
		if (infile == NULL) {
			cw_log(LOG_ERR, "Can't open image %s: %s",
			       sock_addr2str(&conn->addr), strerror(errno));
			return;
		}



		CLOCK_DEFINE(clk);
		cw_clock_start(&clk);

		mbag_item_t *eof = mbag_set_const_ptr(conn->outgoing, CW_ITEM_IMAGE_FILEHANDLE,
					   infile);

		int rc=0;
	        while (conn->capwap_state == CW_STATE_IMAGE_DATA && rc==0 && eof!=NULL) {
			rc = cw_send_request(conn, CW_MSG_IMAGE_DATA_REQUEST);
			eof = mbag_get(conn->outgoing,CW_ITEM_IMAGE_FILEHANDLE);
		}


		if (rc) {
			cw_log(LOG_ERR,"Error sending image to %s: %s",sock_addr2str(&conn->addr),cw_strrc(rc));
		}
		else {
			cw_dbg(DBG_INFO,"Image '%s' sucessful sent to %s in %0.1f seconds.",
				filename,sock_addr2str(&conn->addr),cw_clock_stop(&clk));
			conn->capwap_state=CW_STATE_NONE;
		}
			
		fclose(infile);
		wtpman_remove(wtpman);


}

#include "db.h"
void config_to_sql(struct conn *conn)
{
	// XXX for the moment we use just the IP adress as ID
	char *wtp_id=sock_addr2str(&conn->addr);

	MAVLITER_DEFINE(it,conn->incomming);
	mavliter_foreach(&it){
		mbag_item_t * i = mavliter_get(&it);
		
		const struct cw_itemdef * cwi = cw_itemdef_get(conn->actions->items,i->id,NULL);
		if (cwi){
			DBGX("ID %d,%s",i->id,cwi->id);

	//		printf("%s != %s ?\n",i->type->name,cwi->type->name);
			char str[256];
			if (i->type->to_str){
				i->type->to_str(i,str);
				db_put_wtp_prop(wtp_id,-1,cwi->id,str);
			}
			else{
				cw_log(LOG_ERR,"Can_'t converto to str");

			}
			

		}
		else{
		//	DBGX("ID %d",i->id);

		}

	}
}

static void wtpman_run(void *arg)
{


	struct wtpman *wtpman = (struct wtpman *) arg;
	struct cwrmsg *cwrmsg;	// = conn_get_message(wtpman->conn);

	wtpman->conn->seqnum = 0;
	struct conn *conn = wtpman->conn;


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
	 */

	int rc = 0;
	while (!cw_timer_timeout(timer)
	       && wtpman->conn->capwap_state == CW_STATE_CONFIGURE) {
		rc = cw_read_messages(wtpman->conn);
		if (rc < 0) {
			if (errno != EAGAIN)
				break;
		}
	}

	if (!cw_rcok(rc)) {
		cw_dbg(DBG_INFO, "WTP Problem: %s", cw_strrc(rc));
		wtpman_remove(wtpman);
		return;

	}


	if (conn->capwap_state == CW_STATE_IMAGE_DATA) {
		wtpman_image_data(wtpman);
		return;
	}



	conn->capwap_state=CW_STATE_RUN;

	// XXX testing ...
	DBGX("Cofig to sql","");
	config_to_sql(conn);


	
	rc = 0;
	while (wtpman->conn->capwap_state == CW_STATE_RUN) {
		rc = cw_read_messages(wtpman->conn);
		if (rc < 0) {
			if (errno != EAGAIN)
				break;
		}

		mavl_del_all(conn->outgoing);
		db_get_tasks(conn,sock_addr2str(&conn->addr));



		//printf("Conn: %d\n",conn->outgoing->count);

		if ( conn->outgoing->count ) {
			rc = cw_send_request(conn, CW_MSG_CONFIGURATION_UPDATE_REQUEST);
			DBGX("CU RC: %d",rc);
	
			DBGX("MAV MERGE","");	
			mavl_merge(conn->config,conn->outgoing);
			DBGX("MAV MERGE DONE","");


			printf("Conn aa: %d\n",conn->outgoing->count);
			config_to_sql(conn);
		
		}

			
	
	}




	wtpman_remove(wtpman);
	return;
}


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

	wtpman->conn->strict_capwap = conf_strict_capwap;
	wtpman->conn->strict_hdr = conf_strict_headers;
	wtpman->conn->radios=mbag_i_create();
	wtpman->conn->local = ac_config;
//wtpman->conn->capwap_mode=0; //CW_MODE_STD; //CISCO;
wtpman->conn->capwap_mode=CW_MODE_CISCO;
//wtpman->conn->strict_capwap_hdr=0;

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

//	char wi[4096];
//	wtpinfo_print(wi, &wtpman->wtpinfo);
//	printf("WTPINFO: \n%s\n", wi);



//      char buffer[2048];
//      struct lwmsg lwmsg;
//      lwmsg_init(&lwmsg, buffer,conf_macaddress,LWMSG_DISCOVERY_RESPONSE,conn_get_next_seqnum(wtpman->conn));

//      conn_send_packet(wtpman->conn,buffer,60);




}


void wtpman_start(struct wtpman *wtpman, int dtlsmode)
{
	if (dtlsmode) {
		cw_dbg(DBG_INFO, "Starting wtpman in DTLS mode");
		pthread_create(&wtpman->thread, NULL, (void *) &wtpman_run,
			       (void *) wtpman);
	} else {
		cw_dbg(DBG_INFO, "Starting wtpman in non-dtls mode");
		pthread_create(&wtpman->thread, NULL, (void *) &wtpman_run_discovery,
			       (void *) wtpman);
	}
}


void wtpman_lw_start(struct wtpman *wtpman)
{

}
