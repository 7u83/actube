/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h> // sleep


#include "cw/capwap.h"
#include "cw/sock.h"
#include "cw/log.h"
#include "cw/timer.h"
#include "cw/cw.h"
#include "cw/capwap_items.h"
#include "cw/dtls.h"
#include "cw/dbg.h"
#include "cw/conn.h"
#include "cw/format.h"

#include "ac.h"
#include "conf.h"
#include "db.h"
#include "socklist.h"
#include "wtpman.h"
#include "wtplist.h"


static void reset_echointerval_timer(struct wtpman *wtpman)
{
	uint16_t ct = mbag_get_word(wtpman->conn->local, CW_ITEM_CAPWAP_TIMERS,
				    CAPWAP_MAX_DISCOVERY_INTERVAL << 8 |
				    CAPWAP_ECHO_INTERVAL);

	/* start echinterval timer and put 2 seconds for "safety" on it */

	wtpman->echointerval_timer = cw_timer_start(2+ (ct & 0xff));
	db_ping_wtp(sock_addr2str_p(&wtpman->conn->addr), conf_acname);
//	cw_dbg(DBG_X, "Starting capwap timer: %d", wtpman->echointerval_timer);

}


static int msg_start_handler(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			     int len, struct sockaddr *from)
{
	struct wtpman *wtpman = conn->data;
	reset_echointerval_timer(wtpman);

	return 0;
}




static void wtpman_remove(struct wtpman *wtpman)
{
	wtplist_lock();
	wtplist_remove(wtpman);
	wtplist_unlock();
	wtpman_destroy(wtpman);
}



static void wtpman_run_discovery(void *arg)
{


	struct wtpman *wtpman = (struct wtpman *) arg;

	time_t timer = cw_timer_start(10);

	extern cw_actionlist_in_t the_tree;

	wtpman->conn->capwap_state = CW_STATE_DISCOVERY;
	wtpman->conn->actions = &capwap_actions;

	wtpman->conn->outgoing = mbag_create();
	wtpman->conn->incomming = mbag_create();

	while (!cw_timer_timeout(timer)
	       && wtpman->conn->capwap_state == CW_STATE_DISCOVERY) {
		cw_read_messages(wtpman->conn);
	}

	struct mbag_item *wn = mbag_get(wtpman->conn->incomming, CW_ITEM_WTP_NAME);

	if (wn) {
//              printf("WTP Name: %s\n", wn->data);
//              exit(0);
	}

	wtpman_remove(wtpman);
	return;

}

int xprocess_message(struct conn *conn, uint8_t * rawmsg, int rawlen,
		     struct sockaddr *from)
{
	uint8_t *msgptr = rawmsg + cw_get_hdr_msg_offset(rawmsg);
	uint32_t type = cw_get_msg_type(msgptr);
	cw_log(LOG_ERR, "Hey: %d", type);
	if (type == CW_MSG_DISCOVERY_REQUEST)
		conn->capwap_state = CW_STATE_DISCOVERY;


	return process_message(conn, rawmsg, rawlen, from);
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
		cw_dbg(DBG_DTLS, "Using cert file %s", wtpman->conn->dtls_cert_file);
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
		cw_dbg(DBG_DTLS, "Error establishing DTLS session with %s",
		       sock_addr2str_p(&wtpman->conn->addr));
		return 0;
	}

	cw_dbg(DBG_DTLS, "DTLS session established with %s, cipher=%s",
	       sock_addr2str_p(&wtpman->conn->addr), dtls_get_cipher(wtpman->conn));

	return 1;
}

static int wtpman_join(void *arg, time_t timer)
{
	struct wtpman *wtpman = (struct wtpman *) arg;
	struct conn *conn = wtpman->conn;

	wtpman->conn->outgoing = mbag_create();
	wtpman->conn->incomming = mbag_create();
	conn->config = conn->incomming;
//      wtpman->conn->local = ac_config;

	mbag_set_str(conn->local, CW_ITEM_AC_NAME, conf_acname);



	wtpman->conn->capwap_state = CW_STATE_JOIN;
//	wtpman->conn->actions = &capwap_actions;

//      wtpman->conn->itemstore = mbag_create();


	cw_dbg(DBG_INFO, "Join State - %s", sock_addr2str(&conn->addr));

	int rc;
	while (!cw_timer_timeout(timer) && wtpman->conn->capwap_state == CW_STATE_JOIN) {
		rc = cw_read_messages(wtpman->conn);
		if (rc < 0) {
			break;
		}
	}

	if (rc != 0) {
		cw_log(LOG_ERR, "Error joining WTP %s", cw_strerror(rc));
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
	struct conn *conn = wtpman->conn;

	/* Image upload */
	const char *filename = mbag_get_str(conn->outgoing, CW_ITEM_IMAGE_FILENAME, NULL);
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



	CW_CLOCK_DEFINE(clk);
	cw_clock_start(&clk);

	mbag_item_t *eof = mbag_set_const_ptr(conn->outgoing, CW_ITEM_IMAGE_FILEHANDLE,
					      infile);

	int rc = 0;
	while (conn->capwap_state == CW_STATE_IMAGE_DATA && rc == 0 && eof != NULL) {
		rc = cw_send_request(conn, CW_MSG_IMAGE_DATA_REQUEST);
		eof = mbag_get(conn->outgoing, CW_ITEM_IMAGE_FILEHANDLE);
	}


	if (rc) {
		cw_log(LOG_ERR, "Error sending image to %s: %s",
		       sock_addr2str(&conn->addr), cw_strrc(rc));
	} else {
		cw_dbg(DBG_INFO, "Image '%s' sucessful sent to %s in %0.1f seconds.",
		       filename, sock_addr2str(&conn->addr), cw_clock_stop(&clk));
		conn->capwap_state = CW_STATE_NONE;
	}

	fclose(infile);
	wtpman_remove(wtpman);


}

void props_to_sql(struct conn *conn, mbag_t  mb, const char *mid)
{
	// XXX for the now we use just the IP adress as ID
	char *wtp_id = sock_addr2str(&conn->addr);

//	cw_dbg(DBG_X, "WTPID: %s\n", wtp_id);

	MAVLITER_DEFINE(it, mb);
	mavliter_foreach(&it) {
		mbag_item_t *i = mavliter_get(&it);

		const struct cw_itemdef *cwi;

		if (!mid){
		    cwi = cw_itemdef_get(conn->actions->items, i->id, NULL);
		}
		else{
		    cwi = cw_itemdef_get(conn->actions->items, mid,i->id);
		}

		if (!cwi){
			cw_dbg(DBG_WARN,"No definition for item %s found.",i->id);
			continue;
		}

		if (i->type==MBAG_MBAG){
			if (mid){
				cw_log(LOG_ERROR,"Depth for %s",i->id);
				continue;
			}

			props_to_sql(conn,i->data,i->id);
			continue;
		}	


		DBGX("SQL ID %s,%s", i->id, cwi->id);
		DBGX("SQL Type %s,Typecwd %s", i->type->name, cwi->type->name);

		//              printf("%s != %s ?\n",i->type->name,cwi->type->name);
		char str[256];
		if (i->type->to_str) {
			i->type->to_str(i, str);
			db_put_wtp_prop(wtp_id, cwi->id, cwi->sub_id, str);
		} else {
			cw_log(LOG_ERR, "Can't converto to str for %s", cwi->id,
			       cwi->sub_id);

		}

	}
}




void config_to_sql(struct conn *conn)
{
	// XXX for the moment we use just the IP adress as ID
	char *wtp_id = sock_addr2str(&conn->addr);

//	cw_dbg(DBG_X, "WTPID: %s\n", wtp_id);

	MAVLITER_DEFINE(it, conn->incomming);
	mavliter_foreach(&it) {
		mbag_item_t *i = mavliter_get(&it);

		const struct cw_itemdef *cwi =
		    cw_itemdef_get(conn->actions->items, i->id, NULL);

		cw_dbg(DBG_X,"ID GOT: %s",i->id);

		if (cwi) {
			DBGX("SQL ID %s,%s", i->id, cwi->id);
			DBGX("SQL Type %s,Typecwd %s", i->type->name, cwi->type->name);

			//              printf("%s != %s ?\n",i->type->name,cwi->type->name);
			char str[256];
			if (i->type->to_str) {
				i->type->to_str(i, str);
				db_put_wtp_prop(wtp_id, cwi->id, cwi->sub_id, str);
			} else {
				cw_log(LOG_ERR, "Can't converto to str for %s", cwi->id,
				       cwi->sub_id);

			}


		} else {
			//      DBGX("ID %d",i->id);

		}

	}
}

void radio_to_sql(struct conn *conn, char *wtp_id, int rid, mbag_t radio)
{
	MAVLITER_DEFINE(it, radio);
	mavliter_foreach(&it) {
		mbag_item_t *i = mavliter_get(&it);

		const struct cw_itemdef *cwi =
		    cw_itemdef_get(conn->actions->radioitems, i->id, NULL);
		if (cwi) {
			char str[4096];
			if (i->type->to_str) {
				i->type->to_str(i, str);
//				printf("I would put RID: %d, %s=>%s\n",rid,cwi->id,str);

				char srid[6];
				sprintf(srid,"%d",rid);

				db_put_radio_prop(wtp_id,srid,cwi->id,cwi->sub_id,str);

//				db_put_wtp_prop(wtp_id, cwi->id, cwi->sub_id, str);
			} else {
				cw_log(LOG_ERR, "Can't converto to str for %s", cwi->id,
				       cwi->sub_id);

			}


		} else {
			//      DBGX("ID %d",i->id);

		}

	}


//		int rid = ((struct mbag_item*)mavliter_get(&it))->iid;

}


void radios_to_sql(struct conn *conn)
{
	char *wtp_id = sock_addr2str(&conn->addr);
	MAVLITER_DEFINE(it, conn->radios);
	mavliter_foreach(&it) {
		struct mbag_item * i = mavliter_get(&it);
		int rid = i->iid;

		radio_to_sql(conn,wtp_id,rid,i->data);


	}
}





void wtpman_run_data(void *wtpman_arg)
{

	return;

	struct wtpman *wtpman = (struct wtpman *) wtpman_arg;
	struct conn *conn = wtpman->conn;


	uint8_t data[1001];
	memset(data, 0, 1000);

	cw_log(LOG_ERR, "I am the data thread**********************************************************************\n");
	while (1) {
		sleep(5);
//		conn->write_data(conn, data, 100);
		cw_log(LOG_ERR, "O was the data thread***********************************************************\n");
	}


}


static int msg_end_handler(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			     int len, struct sockaddr *from)
{
	if (a->msg_id ==CW_MSG_CHANGE_STATE_EVENT_REQUEST) {
		props_to_sql(conn,conn->incomming,0);
		radios_to_sql(conn);
	}
}


static void wtpman_run(void *arg)
{


	struct wtpman *wtpman = (struct wtpman *) arg;

	wtpman->conn->seqnum = 0;
	struct conn *conn = wtpman->conn;




	/* reject connections to our multi- or broadcast sockets */
/*	if (socklist[wtpman->socklistindex].type != SOCKLIST_UNICAST_SOCKET) {
		cw_dbg(DBG_DTLS, "Dropping connection from %s to non-unicast socket.",
		       CLIENT_IP);
		wtpman_remove(wtpman);
		return;
	}
*/

	time_t timer = cw_timer_start(wtpman->conn->wait_dtls);

	/* establish dtls session */
/*	if (!wtpman_establish_dtls(wtpman)) {
		wtpman_remove(wtpman);
		return;
	}
*/

	/* dtls is established, goto join state */
	if (!wtpman_join(wtpman, timer)) {
		wtpman_remove(wtpman);
		return;
	}


	conn->msg_start = msg_start_handler;


	cw_dbg(DBG_INFO, "WTP from %s has joined with session id: %s",
			sock_addr2str_p(&conn->addr),
			format_bin2hex(conn->session_id,16));




//	cw_dbg(DBG_INFO, "Creating data thread");
//	pthread_t thread;
//	pthread_create(&thread, NULL, (void *) wtpman_run_data, (void *) wtpman);


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



	conn->capwap_state = CW_STATE_RUN;

	// XXX testing ...
//	DBGX("Cofig to sql", "");
	props_to_sql(conn,conn->incomming,0);
	radios_to_sql(conn);


	conn->msg_end=msg_end_handler;
	/* The main run loop */
	reset_echointerval_timer(wtpman);

	rc = 0;
	while (wtpman->conn->capwap_state == CW_STATE_RUN) {
		rc = cw_read_messages(wtpman->conn);
		if (rc < 0) {
			if (errno != EAGAIN)
				break;
		}

//		cw_dbg(DBG_X, "Time left: %d",
//		       cw_timer_timeleft(wtpman->echointerval_timer));
		if (cw_timer_timeout(wtpman->echointerval_timer)) {

			cw_dbg(DBG_INFO, "Lost connection to WTP:%s",
			       sock_addr2str_p(&conn->addr));
			break;
		}

		mavl_del_all(conn->outgoing);
		conn_clear_upd(conn,1);

//	props_to_sql(conn,conn->incomming,0);
//	radios_to_sql(conn);



		mavl_conststr_t r;
		r = db_get_update_tasks(conn, sock_addr2str(&conn->addr));
		if (r) {

			if (!conn->outgoing->count)
				continue;

			cw_dbg(DBG_INFO, "Updating WTP %s",sock_addr2str(&conn->addr));

			rc = cw_send_request(conn, CW_MSG_CONFIGURATION_UPDATE_REQUEST);
			mavl_merge(conn->config, conn->outgoing);
			mavl_destroy(conn->outgoing);
			conn->outgoing = mbag_create();
			props_to_sql(conn,conn->incomming,0);
			radios_to_sql(conn);
			mavl_destroy(r);
		}

		r = db_get_radio_tasks(conn, sock_addr2str(&conn->addr));
		if (r) {

			if (!conn->radios_upd->count)
				continue;

			cw_dbg(DBG_INFO, "Updating Radios for %s",sock_addr2str(&conn->addr));
			rc = cw_send_request(conn, CW_MSG_CONFIGURATION_UPDATE_REQUEST);


			conn_clear_upd(conn,1);

//			mavl_destroy(conn->radios_upd);
//			conn->radios_upd=mbag_i_create();


			radios_to_sql(conn);

			/*
			rc = cw_send_request(conn, CW_MSG_CONFIGURATION_UPDATE_REQUEST);
			mavl_merge(conn->config, conn->outgoing);
			mavl_destroy(conn->outgoing);
			conn->outgoing = mbag_create();
			config_to_sql(conn);
			radios_to_sql(conn);
			mavl_destroy(r);
			*/
		}



	}

	db_ping_wtp(sock_addr2str_p(&conn->addr), "");
	wtpman_remove(wtpman);
	return;
}


static void wtpman_run_dtls(void *arg)
{

	struct wtpman *wtpman = (struct wtpman *) arg;



	/* reject connections to our multi- or broadcast sockets */
	if (socklist[wtpman->socklistindex].type != SOCKLIST_UNICAST_SOCKET) {
		cw_dbg(DBG_DTLS, "Dropping connection from %s to non-unicast socket.",
		       sock_addr2str_p(&wtpman->conn->addr));
		wtpman_remove(wtpman);
		return;
	}
//      time_t timer = cw_timer_start(wtpman->conn->wait_dtls);

	/* establish dtls session */
	if (!wtpman_establish_dtls(wtpman)) {
		wtpman_remove(wtpman);
		return;
	}

	wtpman_run(arg);
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


	int replyfd;
	if (socklist[socklistindex].type != SOCKLIST_UNICAST_SOCKET) {

		int port = sock_getport(&socklist[socklistindex].addr);
		replyfd = socklist_find_reply_socket(srcaddr, port);

		if (replyfd == -1) {
			cw_log(LOG_ERR, "Can't find reply socket for request from %s",
			       sock_addr2str(srcaddr));
			free(wtpman);
			return NULL;
		}
	} else {
		replyfd = socklist[socklistindex].sockfd;
	}

	int sockfd = replyfd;	//socklist[socklistindex].reply_sockfd;


	struct sockaddr dbgaddr;
	socklen_t dbgaddrl = sizeof(dbgaddr);
	getsockname(sockfd, &dbgaddr, &dbgaddrl);

	cw_dbg(DBG_INFO, "Creating wtpman on socket %d, %s:%d", sockfd,
	       sock_addr2str(&dbgaddr), sock_getport(&dbgaddr));

//extern int conn_process_packet2(struct conn *conn, uint8_t * packet, int len,
//                        struct sockaddr *from);

	wtpman->conn = conn_create(sockfd, srcaddr, 100);

	wtpman->conn->data_sock = socklist[socklistindex].data_sockfd;
	sock_copyaddr(&wtpman->conn->data_addr, (struct sockaddr *) &wtpman->conn->addr);



//      wtpman->conn->process_packet = conn_process_packet2;

	if (!wtpman->conn) {
		wtpman_destroy(wtpman);
		return NULL;
	}

	wtpman->conn->mods = conf_mods;

	wtpman->conn->strict_capwap = conf_strict_capwap;
	wtpman->conn->strict_hdr = conf_strict_headers;
	wtpman->conn->radios = mbag_i_create();
	wtpman->conn->radios_upd = mbag_i_create();
	wtpman->conn->local = ac_config;
//wtpman->conn->capwap_mode=0; //CW_MODE_STD; //CISCO;
	wtpman->conn->capwap_mode = CW_MODE_CISCO;
//wtpman->conn->strict_capwap_hdr=0;

	return wtpman;
}


void wtpman_addpacket(struct wtpman *wtpman, uint8_t * packet, int len)
{
	conn_q_add_packet(wtpman->conn, packet, len);
}


int nodtls = 0;


void wtpman_start(struct wtpman *wtpman, int dtlsmode)
{



	if (dtlsmode) {
		cw_dbg(DBG_INFO, "Starting wtpman in DTLS mode");
		pthread_create(&wtpman->thread, NULL, (void *) wtpman_run_dtls,
			       (void *) wtpman);
	} else {
		cw_dbg(DBG_INFO, "Starting wtpman in non-dtls mode");

		if (nodtls) {
			wtpman->conn->process_message = xprocess_message;
			pthread_create(&wtpman->thread, NULL, (void *) wtpman_run,
				       (void *) wtpman);
			return;

		}

		pthread_create(&wtpman->thread, NULL, (void *) wtpman_run_discovery,
			       (void *) wtpman);
	}
}





void wtpman_lw_addpacket(struct wtpman *wtpman, uint8_t * packet, int len)
{
//      uint8_t * m = packet+12;
//      int l = LWTH_GET_LENGTH(packet+6);

	uint8_t *msg = packet + 12;


	int msgtype = LWMSG_GET_TYPE(msg);
	int msglen = LWMSG_GET_LEN(msg);
	printf("Type is %d, Len is %d\n", msgtype, msglen);

//      uint8_t *msgdata = LWMSG_GET_DATA(msg);

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

	//uint8_t *data;
/*
	lw_foreach_msgelem(data,msgdata,msglen){
		int eltype = LWMSGELEM_GET_TYPE(data);
		int ellen = LWMSGELEM_GET_LEN(data);
		uint8_t * eldata = LWMSGELEM_GET_DATA(data);

		wtpinfo_lwreadelem_wtp_descriptor(&wtpman->wtpinfo,eltype,eldata,ellen);

		printf ("ELEM TYPE: %d, LEN: %d\n",eltype,ellen);

	}
*/

//      char wi[4096];
//      wtpinfo_print(wi, &wtpman->wtpinfo);
//      printf("WTPINFO: \n%s\n", wi);



//      char buffer[2048];
//      struct lwmsg lwmsg;
//      lwmsg_init(&lwmsg, buffer,conf_macaddress,LWMSG_DISCOVERY_RESPONSE,conn_get_next_seqnum(wtpman->conn));

//      conn_send_packet(wtpman->conn,buffer,60);




}


/*
void wtpman_lw_start(struct wtpman *wtpman)
{

}
*/
