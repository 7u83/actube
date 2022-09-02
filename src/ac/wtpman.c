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
#include <unistd.h>


#include "cw/capwap.h"
#include "cw/sock.h"
#include "cw/log.h"
#include "cw/timer.h"
#include "cw/cw.h"

#include "cw/dtls.h"
#include "cw/dbg.h"
#include "cw/conn.h"
#include "cw/format.h"

#include "ac.h"
#include "conf.h"
#include "socklist.h"
#include "wtpman.h"
#include "wtplist.h"
#include "cw/val.h"

#include "actube.h"


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

	wtpman->conn->capwap_state = CAPWAP_STATE_DISCOVERY;


	while (!cw_timer_timeout(timer)
	       && wtpman->conn->capwap_state == CAPWAP_STATE_DISCOVERY) {
		int rc;
		rc = cw_read_messages(wtpman->conn);


		if (cw_result_is_ok(rc)) {
			wtpman->conn->capwap_state = CAPWAP_STATE_JOIN;

			cw_dbg(DBG_INFO, "Discovery has detected mods: %s %s",
			       wtpman->conn->cmod->name,
			       wtpman->conn->bmod->name);

			wtplist_lock();
			discovery_cache_add(discovery_cache,
					    (struct sockaddr *) &wtpman->conn->
					    addr, wtpman->conn->cmod,
					    wtpman->conn->bmod);
			wtplist_unlock();

		}
	}
	return;
}

static int wtpman_dtls_setup(void *arg)
{
	char cipherstr[512];

	char sock_buf[SOCK_ADDR_BUFSIZE];
	struct wtpman *wtpman = (struct wtpman *) arg;

	/* try to accept the connection */
	if (!dtls_accept(wtpman->conn)) {
		cw_dbg(DBG_DTLS, "Error establishing DTLS session with %s",
		       sock_addr2str_p(&wtpman->conn->addr, sock_buf));
		return 0;
	}

	cw_dbg(DBG_DTLS, "DTLS session established with %s, %s",
	       sock_addr2str_p(&wtpman->conn->addr, sock_buf),
	       dtls_get_cipher(wtpman->conn, cipherstr));


	return 1;
}
/*
static int wtpman_join(void *arg)
{
	int rc;
	char sock_buf[SOCK_ADDR_BUFSIZE];
	struct wtpman *wtpman = (struct wtpman *) arg;
	struct cw_Conn *conn = wtpman->conn;
	time_t timer, wait_join;

	cw_dbg(DBG_INFO, "Join State - %s",
	       sock_addr2str(&conn->addr, sock_buf));

	wait_join =
	    cw_ktv_get_word(conn->global_cfg, "wait-join", CAPWAP_WAIT_JOIN);

	timer = cw_timer_start(wait_join);


	while (!cw_timer_timeout(timer)
	       && wtpman->conn->capwap_state == CAPWAP_STATE_JOIN) {
		rc = cw_read_messages(wtpman->conn);
		if (rc < 0) {
			if (errno == EAGAIN)
				continue;

			break;
		}
		cw_dbg_ktv_dump(conn->remote_cfg, DBG_INFO,
				"-------------dump------------",
				"DMP", "---------end dump --------");
	}

	if (rc != 0) {
		cw_log(LOG_ERR, "Error joining WTP %s", cw_strerror(rc));
		return 0;
	}


	if (wtpman->conn->capwap_state != CAPWAP_STATE_JOIN_COMPLETE) {
		cw_dbg(DBG_MSG_ERR,
		       "No join request from %s after %d seconds, WTP died.",
		       sock_addr2str(&wtpman->conn->addr, sock_buf), wait_join);

		return 0;
	}


	return 1;


}
*/



void wtpman_image_data(struct wtpman *wtpman)
{
/*	char sock_buf[SOCK_ADDR_BUFSIZE];
	struct cw_Conn *conn = wtpman->conn;

	// Image upload 
	const char *filename = mbag_get_str(conn->outgoing, CW_ITEM_IMAGE_FILENAME, NULL);
	if (!filename) {
		cw_log(LOG_ERR,
		       "Can't send image to %s. No Image Filename Item found.",
		       sock_addr2str(&conn->addr,sock_buf));
		return;
	}
	cw_dbg(DBG_INFO, "Sending image file '%s' to '%s'.", filename,
	       sock_addr2str(&conn->addr,sock_buf));
	FILE *infile = fopen(filename, "rb");
	if (infile == NULL) {
		cw_log(LOG_ERR, "Can't open image %s: %s",
		       sock_addr2str(&conn->addr,sock_buf), strerror(errno));
		return;
	}



	CW_CLOCK_DEFINE(clk);
	cw_clock_start(&clk);

	mbag_item_t *eof = mbag_set_const_ptr(conn->outgoing, CW_ITEM_IMAGE_FILEHANDLE,
					      infile);

	int rc = 0;
	while (conn->capwap_state == CW_STATE_IMAGE_DATA && rc == 0 && eof != NULL) {
		rc = cw_send_request(conn, CAPWAP_MSG_IMAGE_DATA_REQUEST);
		eof = mbag_get(conn->outgoing, CW_ITEM_IMAGE_FILEHANDLE);
	}


	if (rc) {
		cw_log(LOG_ERR, "Error sending image to %s: %s",
		       sock_addr2str(&conn->addr,sock_buf), cw_strrc(rc));
	} else {
		cw_dbg(DBG_INFO, "Image '%s' sucessful sent to %s in %0.1f seconds.",
		       filename, sock_addr2str(&conn->addr,sock_buf), cw_clock_stop(&clk));
		conn->capwap_state = CW_STATE_NONE;
	}

	fclose(infile);
	wtpman_remove(wtpman);
*/

}








void *wtpman_run_data(void *wtpman_arg)
{

	return NULL;
/*
	struct wtpman *wtpman = (struct wtpman *) wtpman_arg;
	struct cw_Conn *conn = wtpman->conn;


	uint8_t data[1001];
	memset(data, 0, 1000);

	cw_log(LOG_ERR, "I am the data thread**********************************************************************\n");
	while (1) {
		sleep(5);
/ //		conn->write_data(conn, data, 100); /
		cw_log(LOG_ERR, "O was the data thread***********************************************************\n");
	}
*/

}



int cw_run_state_machine(struct cw_Conn *conn, time_t * timer)
{

	int timerval;
	cw_StateMachineState_t search, *result;


	while (1) {
		search.state = conn->capwap_state;
		search.prevstate = conn->capwap_prevstate;
		result = mavl_get(conn->msgset->statemachine_states, &search);

		cw_dbg(DBG_STATE, "State transition: [%s -> %s]",
		       cw_strstate(conn->capwap_prevstate),
		       cw_strstate(conn->capwap_state)
		    );
		if (result == NULL) {
			cw_log(LOG_ERR, "State not found");
			return 0;
		}
		if (result->jump_state) {
			conn->capwap_state = result->jump_state;
			conn->capwap_prevstate = result->jump_prevstate;

			cw_dbg(DBG_STATE, "Jump to state: [%s->%s]",
			       cw_strstate(conn->capwap_prevstate),
			       cw_strstate(conn->capwap_state));
			continue;
		}

		if (result->dbgmsg) {
			cw_dbg(DBG_STATE, "%s", result->dbgmsg);
		}

		if (result->timer_key) {
			timerval =
			    cw_cfg_get_word(conn->local_cfg, result->timer_key,
					    result->timer_default);
			*timer = cw_timer_start(timerval);
			cw_dbg(DBG_STATE, "Starting timer: [%s] - %d seconds.",
			       result->timer_key, timerval);
		}
		return result->retval;
	}
}


/*#define CW_TRANSITION(prestate,state) (prestate<<16|state)*/


int run_update(struct wtpman *wtpman)
{
	int rc;
	if (!wtpman->update)
		return EAGAIN;
	rc = cw_send_request(wtpman->conn, CAPWAP_MSG_CONFIGURATION_UPDATE_REQUEST);
        cw_cfg_clear(wtpman->conn->update_cfg);
	wtpman->update=0;
	return rc;
}

static void *wtpman_main(void *arg)
{
	//mavl_t r;
	int rc;
	time_t timer;
	//char sock_buf[SOCK_ADDR_BUFSIZE];
	struct cw_Conn *conn;
//	int last_state;
	struct wtpman *wtpman = (struct wtpman *) arg;

	wtpman->conn->seqnum = 0;
	conn = wtpman->conn;

//	wtpman->conn->remote_cfg = cw_cfg_create();


	if (!wtpman->dtlsmode) {
		/* We were invoked with an unencrypted packet, 
		 * so assume, it is a discovery request */
		wtpman_run_discovery(arg);
		wtpman_remove(wtpman);
		return NULL;
	}


	/* reject connections to our multi- or broadcast sockets */
	if (socklist[wtpman->socklistindex].type != SOCKLIST_UNICAST_SOCKET) {
		cw_dbg(DBG_DTLS, "Reject multi");
		wtpman_remove(wtpman);
		return NULL;
	}

	conn->capwap_state = CAPWAP_STATE_DTLS_SETUP;
	/* establish dtls session */
	if (!wtpman_dtls_setup(wtpman)) {
		wtpman_remove(wtpman);
		return NULL;
	}

	/*last_state = conn->capwap_state;
	   conn->capwap_state = CAPWAP_STATE_JOIN;
	 */
	conn->capwap_prevstate = CAPWAP_STATE_DTLS_SETUP;
	conn->capwap_state = CAPWAP_STATE_JOIN;
	rc = 0;
	wtpman->update=0;

	while (1) {

		//int wait_join;
		//int wait_change_state;



		if (!cw_run_state_machine(conn, &timer)) {
			cw_dbg(DBG_INFO, "WTP died");
			wtpman_remove(wtpman);
			return NULL;
		};


		while (!cw_timer_timeout(timer)) {
			rc = run_update(wtpman);
			if (rc !=EAGAIN)
				break;

			rc = cw_read_messages(wtpman->conn);
			
			if (rc < 0) {
				if (errno == EAGAIN){
					continue;
				}
			}
			break;
		}
		if (rc < 0) {
			conn->capwap_prevstate = conn->capwap_state;
			conn->capwap_state = CAPWAP_STATE_TIMEOUT;
		}

	}
	exit(0);

	return NULL;
}



void wtpman_destroy(struct wtpman *wtpman)
{
	if (wtpman->conn)
		conn_destroy(wtpman->conn);
	
	if (wtpman->wtp_cfg)
		cw_cfg_destroy(wtpman->wtp_cfg);

	free(wtpman);
}


static void copy(struct cw_ElemHandlerParams * params)
{
        cw_cfg_copy(params->cfg, params->conn->remote_cfg,DBG_CFG_UPDATES,"GlobalCfg");
}

static void catch_cfg(struct cw_ElemHandlerParams * params, int create, const char *format, ...)
{
	const char *wtpname;
	char filename[200];
	FILE *f;
	cw_Cfg_t * cfg_list[3];

	if (!cw_cfg_get_bool(params->conn->global_cfg,"actube/save-initial-wtp-config",0))
		return;

	cfg_list[0]=params->cfg;
	cfg_list[1]=params->conn->remote_cfg;
	cfg_list[2]=NULL;

	wtpname = cw_cfg_get_l(cfg_list,"capwap/wtp-name","default");
	sprintf(filename,"wtp-initial-%s.ckv",wtpname);

	if (create)		
		f = fopen(filename,"w");
	else
		f = fopen(filename,"a");
	if (f==NULL)
		return;

	if (create)
		fprintf(f,"#\n# Initial config for WTP '%s' - catched by ACTube\n#\n\n",wtpname);

        if (format !=NULL){
                va_list args;
                va_start(args,format);
                vfprintf(f,format,args);
                va_end(args);
        }
	cw_cfg_write_to_file(f, params->cfg);
	fclose(f);



}



static int discovery_cb(struct cw_ElemHandlerParams * params, struct cw_MsgCb_data * d)
{

	cw_dbg(DBG_X,"DISCOVERY Callback");
	copy(params);
	catch_cfg(params,1,"\n#\n# Discovery Request\n#\n");

	cw_cfg_clear(params->cfg);
	if (d->parent)
		return d->parent->fun(params,d->parent);

	return 0;
}

static int join_cb(struct cw_ElemHandlerParams * params, struct cw_MsgCb_data *d)
{
	catch_cfg(params,0,"\n#\n# Join Request\n#\n");
	if (d->parent)
		return d->parent->fun(params,d->parent);
	return 0;
}


/*
static int fill_update_cfg(struct cw_Conn * conn)
{
	struct cw_Cfg_iter cfi;
	struct cw_Cfg_entry *e;
	cw_Cfg_t * u;
	u=cw_cfg_create();
	if( cw_cfg_load("status_response.ckv",u)){
		cw_log(LOG_ERR,"Cant load file");
		stop();
	};
	cw_cfg_dump(conn->remote_cfg);
        cw_cfg_iter_init(u, &cfi, NULL);
	while ((e = cw_cfg_iter_next(&cfi, NULL))!=NULL){
		const char * r;
		r = cw_cfg_get(conn->remote_cfg, e->key, "[]");
	//	cw_dbg(DBG_CFG_UPDATES,"check: %s: %s",e->key,e->val);
		if (strcmp(r,e->val)==0)
			continue;

		cw_dbg(DBG_CFG_UPDATES,"Status reps: %s: %s -> %s",e->key,r,e->val);
	}

	return 0;
}
*/

static int cfg_status_cb(struct cw_ElemHandlerParams * params, struct cw_MsgCb_data *d)
{
	catch_cfg(params,0,"\n#\n# Configuration Status Request\n#\n");
	if (d->parent)
		return d->parent->fun(params,d->parent);
	return 0;
}

static int event_cb(struct cw_ElemHandlerParams * params, struct cw_MsgCb_data *d)
{
	struct cw_Conn * conn = (struct cw_Conn*)params->conn;
	struct wtpman * wtpman = (struct wtpman *)conn->data;

	char filename[200];


	
	cw_dbg(DBG_X,"WTP EVENT Callback");
	copy(params);

	const char * wtpname = cw_cfg_get(conn->remote_cfg,"capwap/wtp-name","default");
	sprintf(filename,"wtp-event-%d-%s.ckv",wtpman->ctr++,wtpname);
	cw_cfg_save(filename,params->cfg,NULL);
	return 0;
}

static int change_state_event_cb(struct cw_ElemHandlerParams * params,struct cw_MsgCb_data * d)
{
	struct cw_Conn * conn = (struct cw_Conn*)params->conn;
	struct wtpman * wtpman = (struct wtpman *)conn->data;

	char filename[200];


	
	cw_dbg(DBG_X,"WTP EVENT Callback");
	copy(params);

	const char * wtpname = cw_cfg_get(conn->remote_cfg,"capwap/wtp-name","default");
	sprintf(filename,"wtp-change-event-%d-%s.ckv",wtpman->ctr++,wtpname);
	cw_cfg_save(filename,params->cfg,NULL);
	return 0;
}





struct wtpman *wtpman_create(int socklistindex, struct sockaddr *srcaddr,
			     int dtlsmode, cw_Cfg_t * global_cfg)
{
	struct sockaddr dbgaddr;
	socklen_t dbgaddrl;
	int sockfd, replyfd;
	char sock_buf[SOCK_ADDR_BUFSIZE];

	struct wtpman *wtpman;
	wtpman = malloc(sizeof(struct wtpman));
	if (!wtpman)
		return 0;

	wtpman->ctr=0;

	if (socklist[socklistindex].type != SOCKLIST_UNICAST_SOCKET) {

		int port = sock_getport(&socklist[socklistindex].addr);
		replyfd = socklist_find_reply_socket(srcaddr, port);

		if (replyfd == -1) {
			cw_log(LOG_ERR,
			       "Can't find reply socket for request from %s",
			       sock_addr2str(srcaddr, sock_buf));
			free(wtpman);
			return NULL;
		}
	} else {
		replyfd = socklist[socklistindex].sockfd;
	}

	sockfd = replyfd;	/*//socklist[socklistindex].reply_sockfd; */


	dbgaddrl = sizeof(dbgaddr);
	getsockname(sockfd, &dbgaddr, &dbgaddrl);

	cw_dbg(DBG_INFO, "Creating wtpman on socket %d, %s:%d", sockfd,
	       sock_addr2str(&dbgaddr, sock_buf), sock_getport(&dbgaddr));


	memset(wtpman, 0, sizeof(struct wtpman));

	wtpman->global_cfg = global_cfg;

	wtpman->conn = cw_conn_create(sockfd, srcaddr, 100);
	if (!wtpman->conn) {
		wtpman_destroy(wtpman);
		return NULL;
	}

	cw_conn_register_msg_cb(wtpman->conn,
		CAPWAP_MSG_DISCOVERY_REQUEST,
		discovery_cb);


	wtpman->conn->global_cfg = global_cfg;
	wtpman->conn->local_cfg = cw_cfg_create();
	wtpman->wtp_cfg = cw_cfg_create();


	wtpman->conn->role = CW_ROLE_AC;
	wtpman->conn->data=wtpman;

	wtpman->conn->cfg_list[0]=wtpman->conn->update_cfg;
	wtpman->conn->cfg_list[1]=wtpman->conn->remote_cfg;
	wtpman->conn->cfg_list[2]=wtpman->conn->local_cfg;
	wtpman->conn->cfg_list[3]=wtpman->conn->global_cfg;
	wtpman->conn->cfg_list[4]=NULL;


	wtpman->conn->data_sock = socklist[socklistindex].data_sockfd;
	sock_copyaddr(&wtpman->conn->data_addr,
		      (struct sockaddr *) &wtpman->conn->addr);



//	wtpman->conn->mods = conf_mods;

	wtpman->conn->strict_capwap = conf_strict_capwap;
	wtpman->conn->strict_hdr = conf_strict_headers;

//	wtpman->conn->setup_complete = setup_complete;
/*
//	wtpman->conn->radios = mbag_i_create();
//	wtpman->conn->radios_upd = mbag_i_create();
//	wtpman->conn->local = ac_config;
//wtpman->conn->capwap_mode=0; //CW_MODE_STD; //CISCO;
//	wtpman->conn->capwap_mode = CW_MODE_CISCO;
//wtpman->conn->strict_capwap_hdr=0;
*/



	/* when created caused by a packet in DTLS mode, we try
	 * to find out the modules to load, for detected connection 
	 * from discovery request */
	if (dtlsmode) {
		int rc;
		struct cw_Mod *cmod, *bmod;

		rc = discovery_cache_get(discovery_cache, srcaddr, &cmod,
					 &bmod);
		if (rc) {
			cw_dbg(DBG_INFO, "Initializing with mod %s %s",
			       cmod->name, bmod->name);
			wtpman->conn->msgset =
			    cw_mod_get_msg_set(wtpman->conn, cmod, bmod);
			wtpman->conn->detected = 1;
			cmod->setup_cfg(wtpman->conn);
//	               if (wtpman->conn->setup_complete)
//        	                wtpman->conn->setup_complete(wtpman->conn);


			cw_conn_register_msg_cb(wtpman->conn,
				CAPWAP_MSG_JOIN_REQUEST,
				join_cb);

		cw_conn_register_msg_cb(wtpman->conn,
				CAPWAP_MSG_CONFIGURATION_STATUS_REQUEST,
				cfg_status_cb);

		cw_conn_register_msg_cb(wtpman->conn,
				CAPWAP_MSG_WTP_EVENT_REQUEST,
				event_cb);

/*		cw_conn_register_msg_cb(wtpman->conn,
				CAPWAP_MSG_CHANGE_STATE_EVENT_REQUEST,
				change_state_event_cb);*/



		}
	}



	cw_dbg(DBG_X,"WTPMAN_CREATED: %p",wtpman);

	return wtpman;
}


void wtpman_addpacket(struct wtpman *wtpman, uint8_t * packet, int len)
{
	cw_dbg(DBG_X,"ADD PACKET DETECTED %d",wtpman->conn->detected);
	conn_q_add_packet(wtpman->conn, packet, len);
}

void wtpman_start(struct wtpman *wtpman, int dtlsmode)
{
	cw_dbg(DBG_INFO, "Starting wtpman, DTLS mode = %d", dtlsmode);
	wtpman->dtlsmode = dtlsmode;
	pthread_create(&wtpman->thread, NULL, wtpman_main, (void *) wtpman);
	return;
}
