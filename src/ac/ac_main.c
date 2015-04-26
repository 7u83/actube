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
#include <errno.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#include "actube.h"
#include "wtplist.h"
#include "capwap/dtls.h"
#include "capwap/log.h"
#include "capwap/dbg.h"
#include "conf.h"
#include "capwap/sock.h"

#include "socklist.h"

#include "db.h"
#include "capwap/capwap_items.h"
#include "capwap/capwap_cisco.h"
#include "capwap/capwap_80211.h"

#include "ac.h"
#include "capwap/format.h"

int ac_run();

void * alive_thread(void *data)
{
	while(1){
		sleep(5);
		db_ping();
	}
}

#include "capwap/action.h"


/*
int readelem_vendor_specific_payload(struct conn *conn,struct cw_action * a,uint8_t *data,int len)
{
        cw_action_t as,*af;
	as = *a;

	as.vendor_id = cw_get_dword(data);
	as.elem_id = cw_get_word(data+4);
	printf("Vendor Specific: %d, %d\n",as.vendor_id,as.elem_id);

 	af = cw_actionlist_get(conn->msgtr,&as);



	if (!af) {
		printf("Msg unknown\n");
		return 0;
	}

	printf("Found\n");

	if (af->start) {
		af->start(conn,af,data+6,len-6);
	}


	return 1;
}
*/


/*
int readelem_cisco_rad_name(struct conn *conn,struct cw_action * a,uint8_t *data,int len)
{
	printf("Here is the rad name reader\n");
	int i;

	for (i=0; i<len; i++) {
		printf("%c",data[i]);
	}
	printf("\n");
}
*/


#include "capwap/capwap_crypto.h"


#include "capwap/mbag.h"

int main (int argc, const char * argv[]) 
{

	mbag_t data = mbag_create();

	mbag_set_byte(data,1,25);
	mbag_set_byte(data,7,125);

	int b = mbag_get_byte(data,1,255);
	printf("Result: %d\n",b);

	mbag_del(data,1);
	b = mbag_get_byte(data,1,255);
	printf("Result: %d\n",b);

	//return 0;





/*
	intavltree_t t = intavltree_create();
	int i;
	for (i=0; i<100; i++){
		intavltree_add(t,i);
	}

	avliter_t iter;
	avliter_init(&iter,t);
	int *val;


	for (avliter_seek_set(&iter); val = avliter_get(&iter); avliter_next(&iter)){


		printf("Val is: %d\n",*val);

	}



	exit(0);
*/
	cw_log_name="AC-Tube";

	read_config("ac.conf");



	
	cw_log(LOG_INFO,"Starting AC-Tube, Name=%s, ID=%s",conf_acname,conf_acid);

	cw_dbg_opt_display=DBG_DISP_ASC_DMP | DBG_DISP_COLORS;

	DBGX("Attention! %s","DBGX is ON!");

	/* Locad CAPWAP base protocol */
	if (conf_capwap_mode==CW_MODE_CIPWAP){
		cw_dbg(DBG_INFO,"Locading CIPWAP Aactions");
		cw_register_actions_cipwap_ac(&capwap_actions);
	}
	else {
		cw_dbg(DBG_INFO,"Locading standard CAPWAP Aactions");
		cw_register_actions_capwap_ac(&capwap_actions);
	}

	/* Bindings */
	cw_register_actions_capwap_80211_ac(&capwap_actions);

	
	//cw_register_actions_capwap_80211_ac(&capwap_actions);


/*
	struct avltree *avlt = capwap_actions.strelem;

	avltree_foreach_asc(avlt,avlprint,0);

	exit(0);
*/



	ac_global_init();

	db_init();
	db_start();
	db_ping();
	pthread_t alth;
	pthread_create (&alth, NULL, alive_thread, NULL);
	
	int rc=0;
	dtls_init();
	if (!socklist_init())
		goto errX;

	if (!wtplist_init())
		goto errX;


printf("Debug oprtions: %08X\n",cw_dbg_opt_level);
	rc = ac_run();
errX:
	wtplist_destroy();
	socklist_destroy();
	return rc;
}







void process_ctrl_packet(int  index, struct sockaddr * addr, uint8_t * buffer, int len);
#define AC_PROTO_CAPWAP 0
#define AC_PROTO_LWAPP 1




int ac_run()
{

	if (!conf_listen_addrs_len){
		cw_log(LOG_ERR,"Fatal error: No listen addresses found.");
		return 1;
	}



	/* it is important to create the unicast sockets first, 
	 * because when we create the mcast an bcast sockets next 
	 * we will look for already created sockets to find a
	 * good unicast reply socket */

	int i;
	for(i=0; i<conf_listen_addrs_len; i++){
		socklist_add_unicast(conf_listen_addrs[i],conf_control_port,AC_PROTO_CAPWAP);
#ifdef WITH_LWAPP
		if (conf_lwapp) 
			socklist_add_unicast(conf_listen_addrs[i],conf_lw_control_port,AC_PROTO_LWAPP);
#endif 
	}

	if (socklist_len==0){
		cw_log(LOG_ERR,"Fatal error: Could not setup any listen socket");
		return 1;
	}

	/* create multicast sockets */
	for (i=0; i<conf_mcast_groups_len;i++){
		socklist_add_multicast(conf_mcast_groups[i],conf_control_port,AC_PROTO_CAPWAP);
#ifdef WITH_LWAPP
		if (conf_lwapp) 
			socklist_add_multicast(conf_mcast_groups[i],conf_lw_control_port,AC_PROTO_LWAPP);
#endif 

	}

	/* broadcast sockety ipv4 only */
	for (i=0; i<conf_bcast_addrs_len;i++){
		socklist_add_broadcast(conf_bcast_addrs[i],conf_control_port,AC_PROTO_CAPWAP);
#ifdef WITH_LWAPP
//		printf("Adding %d\n",socklist_len);
		if (conf_lwapp) 
			socklist_add_broadcast(conf_bcast_addrs[i],conf_lw_control_port,AC_PROTO_LWAPP);
//		printf ("SI %d, PROTO: %d\n",socklist_len-1,socklist[socklist_len-1].ac_proto);
#endif 
	}



	//get_acinfo();


	while(1){

		/* prepare fdset */
		fd_set fset;
		int max = 0;
		FD_ZERO(&fset);
		for (i=0; i<socklist_len; i++){
			FD_SET(socklist[i].sockfd,&fset);
			if (socklist[i].sockfd>max)
				max=socklist[i].sockfd;
		}

		/* wait for an event */
		int n;
	        while((n=select(max+1, &fset, NULL, NULL, NULL)) < 0) {
			if (errno != EINTR) 
				return n;

	        }

		/* process the received packet */
		for( i=0; i<socklist_len; i++){

			if (!FD_ISSET(socklist[i].sockfd,&fset))
				continue;

			struct sockaddr_storage srcaddr;
			socklen_t sockaddrlen;

			memset(&srcaddr,0,sizeof(struct sockaddr_storage));
			sockaddrlen = sizeof(struct sockaddr_storage);

			uint8_t buffer[4096];
			int len = sock_receive(socklist[i].sockfd,
					buffer, sizeof(buffer),
					0,
					(struct sockaddr*)&srcaddr, &sockaddrlen);

			process_ctrl_packet(i, (struct sockaddr*)&srcaddr,buffer,len);
		}

	}


	/* close and free all sockts */
	for(i=0; i<socklist_len; i++){
//		close(socklist[i]);
	}
	free(socklist);
	return 0;
}


void process_cw_ctrl_packet(int index,struct sockaddr * addr, uint8_t * buffer, int len)
{

	/* first of all check preamble */
	int preamble = cw_get_hdr_preamble(buffer);

	if (preamble != CAPWAP_PACKET_PREAMBLE && preamble != CAPWAP_DTLS_PACKET_PREAMBLE){
		cw_dbg(DBG_PKT_ERR,"Discarding packet from %s, wrong preamble, preamble = 0x%01X",sock_addr2str(addr),preamble);
		return;
	}


	wtplist_lock();
	struct wtpman * wtpman = wtplist_get(addr);
	if (!wtpman){

		wtpman = wtpman_create(index,addr);


		if (!wtpman ){
			cw_log(LOG_ERR,"Error creating wtpman: %s",strerror(errno));
			wtplist_unlock();
			return;
		}
		

		if (!wtplist_add(wtpman)){
			cw_log(LOG_ERR,"Error adding wtpman: Too many wtp connections");
			wtpman_destroy(wtpman);
			wtplist_unlock();
			return;
		};

		wtpman_start(wtpman,preamble & 0xf);
	}


	wtpman_addpacket(wtpman,buffer,len);
	wtplist_unlock();
}



void process_lw_ctrl_packet(int index,struct sockaddr * addr, uint8_t * buffer, int len)
{
	//int sock = socklist[index].reply_sockfd;

	uint8_t * m = buffer+6;
	uint32_t val = ntohl(*((uint32_t*)(m)));


	printf ("VAL: %08X\n",val);

	

	/* first of all check preamble */
	int version = LWTH_GET_VERSION(m);

	if (version != LW_VERSION){
//		cw_log_debug1("Discarding LWAPP packet, wrong verson");
		return;
	}

	int l = LWTH_GET_LENGTH(m);
	printf ("LEN = %d\n",l);

	if (l+12 != len){
	//	cw_log_debug1("Discarding LWAPP packet, wrong length");
		return;
	}

	wtplist_lock();
	struct wtpman * wtpman = wtplist_get(addr);
	if (!wtpman){

		wtpman = wtpman_create(index,addr);

		if (!wtpman ){
			cw_log(LOG_ERR,"Error creating wtpman: %s",strerror(errno));
			wtplist_unlock();
			return;
		}
		

		if (!wtplist_add(wtpman)){
			cw_log(LOG_ERR,"Error adding wtpman: Too many wtp connections");
			wtpman_destroy(wtpman);
			wtplist_unlock();
			return;
		};

		//wtpman_lw_start(wtpman);
	}

	//wtpman_lw_addpacket(wtpman,buffer,len);
	wtplist_unlock();
}

















void process_ctrl_packet(int index,struct sockaddr * addr, uint8_t * buffer, int len)
{
	switch (socklist[index].ac_proto){
		case AC_PROTO_CAPWAP:
			process_cw_ctrl_packet(index,addr,buffer,len);
			return;
		case AC_PROTO_LWAPP:
			process_lw_ctrl_packet(index,addr,buffer,len);
			return;
	}
}



